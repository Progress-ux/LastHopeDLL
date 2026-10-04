#include "core/last_hope.h"
#include "player_deaththink_hook.h"

#include <link.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>

#include "sdk_util.h"
#include "util/logger.h"

namespace
{
    /*
     * CBasePlayer::PlayerDeathThink()
     *
     * From the current ReGameDLL build:
     *
     *   001415f0 T CBasePlayer::PlayerDeathThink()
     *
     * This is an RVA inside cs.so.
     */
    constexpr uintptr_t PLAYER_DEATH_THINK_RVA = 0x001415f0;

    /*
     * Current function prologue:
     *
     *   1415f0: e8 17 2d f9 ff
     *   1415f5: 05 ff 79 19 00
     *
     * The first instruction calls __x86.get_pc_thunk.ax,
     * and the second instruction calculates the GOT address.
     *
     * We replace exactly these 10 bytes.
     */
    constexpr std::size_t PATCH_SIZE = 10;

    /*
     * After the two stolen instructions execution continues here:
     *
     *   1415fa: 8d 4c 24 04       lea 0x4(%esp),%ecx
     */
    constexpr std::size_t CONTINUE_OFFSET = 10;

    /*
     * Current GOT calculation:
     *
     *   EAX = address of instruction after CALL
     *   EAX += 0x1979ff
     *
     * The instruction after CALL is 0x1415f5.
     *
     * Therefore:
     *
     *   0x1415fa + 0x1979ff = 0x2d8ff9
     *
     * Runtime GOT address = cs_base + 0x2d8ff9.
     */
    constexpr uintptr_t GOT_RVA = 0x002D8FF4;

    using PlayerDeathThinkFn = void (*)(void* player);

    using PlayerDeathThinkHandler =
        bool (*)(void* player);

    struct State
    {
        uintptr_t cs_base = 0;
        uintptr_t target = 0;

        void* trampoline = nullptr;
        std::size_t trampoline_size = 0;

        unsigned char original[PATCH_SIZE] = {};

        bool installed = false;
    };

    State g_state;

    uintptr_t GetModuleBase()
    {
        struct Context
        {
            uintptr_t base = 0;
        };

        Context context;

        dl_iterate_phdr(
            [](struct dl_phdr_info* info, size_t, void* data) -> int
            {
                auto* context =
                    static_cast<Context*>(data);

                if (!info->dlpi_name)
                    return 0;

                const char* name = info->dlpi_name;

                const char* slash =
                    std::strrchr(name, '/');

                const char* basename =
                    slash ? slash + 1 : name;

                if (std::strcmp(basename, "cs.so") != 0)
                    return 0;

                context->base =
                    static_cast<uintptr_t>(info->dlpi_addr);

                return 1;
            },
            &context
        );

        return context.base;
    }

    bool SetPageProtection(
        uintptr_t address,
        std::size_t size,
        int prot
    )
    {
        const long page_size =
            sysconf(_SC_PAGESIZE);

        if (page_size <= 0)
            return false;

        const uintptr_t page_mask =
            static_cast<uintptr_t>(page_size - 1);

        const uintptr_t page_start =
            address & ~page_mask;

        const uintptr_t page_end =
            (address + size + page_mask) & ~page_mask;

        return mprotect(
            reinterpret_cast<void*>(page_start),
            page_end - page_start,
            prot
        ) == 0;
    }

    /*
     * Absolute jump without touching a general-purpose register:
     *
     *     push imm32
     *     ret
     *
     * 68 xx xx xx xx
     * c3
     *
     * 6 bytes.
     */
    void WriteAbsoluteJump(
        unsigned char* destination,
        uintptr_t target
    )
    {
        destination[0] = 0x68;

        const uint32_t address =
            static_cast<uint32_t>(target);

        std::memcpy(
            destination + 1,
            &address,
            sizeof(address)
        );

        destination[5] = 0xc3;
    }

    /*
     * Create a special trampoline for PlayerDeathThink.
     *
     * We cannot simply copy:
     *
     *     call __x86.get_pc_thunk.ax
     *     add $0x1979ff,%eax
     *
     * because both instructions depend on their original address.
     *
     * Instead we reproduce their resulting state directly:
     *
     *     mov eax, runtime_GOT_address
     *
     * and then jump to:
     *
     *     target + 10
     *
     * The original stack state is unchanged, which is equivalent
     * to the CALL/RET pair.
     */
    bool CreateTrampoline()
    {
        constexpr std::size_t MOV_EAX_SIZE = 5;
        constexpr std::size_t PUSH_RET_SIZE = 6;

        g_state.trampoline_size =
            MOV_EAX_SIZE +
            PUSH_RET_SIZE;

        void* memory = mmap(
            nullptr,
            g_state.trampoline_size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0
        );

        if (memory == MAP_FAILED)
        {
            g_state.trampoline = nullptr;
            g_state.trampoline_size = 0;
            return false;
        }

        auto* trampoline =
            static_cast<unsigned char*>(memory);

        /*
         * mov eax, runtime GOT address
         *
         * B8 xx xx xx xx
         */
        trampoline[0] = 0xb8;

        const uintptr_t got_address =
            g_state.cs_base + GOT_RVA;

        const uint32_t got32 =
            static_cast<uint32_t>(got_address);

        std::memcpy(
            trampoline + 1,
            &got32,
            sizeof(got32)
        );

        /*
         * push target + 10
         * ret
         */
        WriteAbsoluteJump(
            trampoline + MOV_EAX_SIZE,
            g_state.target + CONTINUE_OFFSET
        );

        if (!SetPageProtection(
                reinterpret_cast<uintptr_t>(trampoline),
                g_state.trampoline_size,
                PROT_READ | PROT_EXEC))
        {
            munmap(
                trampoline,
                g_state.trampoline_size
            );

            g_state.trampoline = nullptr;
            g_state.trampoline_size = 0;

            return false;
        }

        __builtin___clear_cache(
            reinterpret_cast<char*>(trampoline),
            reinterpret_cast<char*>(trampoline) +
                g_state.trampoline_size
        );

        g_state.trampoline = trampoline;

        return true;
    }

    void DestroyTrampoline()
    {
        if (!g_state.trampoline)
            return;

        munmap(
            g_state.trampoline,
            g_state.trampoline_size
        );

        g_state.trampoline = nullptr;
        g_state.trampoline_size = 0;
    }

    /*
     * This is the actual replacement for:
     *
     *     CBasePlayer::PlayerDeathThink()
     *
     * EXT_FUNC is force_align_arg_pointer in ReGameDLL.
     *
     * We use the same attribute here because the original function
     * has the same stack-alignment requirement.
     */
    void __attribute__((force_align_arg_pointer))
    PlayerDeathThink_Detour(void* player)
    {
        LH_DEBUG("[PlayerDeathThink] DETOUR player=%p", player);

        if (g_last_hope_pending && g_last_hope_player_id > 0)
        {
            if (g_last_hope_player_id)
            {
                if (LastHope_TryRespawnPlayer(g_last_hope_player_id))
                {
                    g_last_hope_pending = false;
                    g_last_hope_player_id = -1;
                    return; // не вызываем trampoline
                }
            }
        }

        auto original = reinterpret_cast<PlayerDeathThinkFn>(g_state.trampoline);
        original(player);
    }

    bool Patch()
    {
        if (!SetPageProtection(
                g_state.target,
                PATCH_SIZE,
                PROT_READ | PROT_WRITE | PROT_EXEC
            ))
        {
            LH_ERROR(
                "[PlayerDeathThink] mprotect(RWX) failed"
            );

            return false;
        }

        auto* target =
            reinterpret_cast<unsigned char*>(
                g_state.target
            );

        /*
         * Replace first 10 bytes:
         *
         *     call ...
         *     add ...
         *
         * with:
         *
         *     push PlayerDeathThink_Detour
         *     ret
         *     nop
         *     nop
         *     nop
         *     nop
         */
        WriteAbsoluteJump(
            target,
            reinterpret_cast<uintptr_t>(
                &PlayerDeathThink_Detour
            )
        );

        for (std::size_t i = 6; i < PATCH_SIZE; ++i)
            target[i] = 0x90;

        __builtin___clear_cache(
            reinterpret_cast<char*>(target),
            reinterpret_cast<char*>(target) +
                PATCH_SIZE
        );

        if (!SetPageProtection(
                g_state.target,
                PATCH_SIZE,
                PROT_READ | PROT_EXEC
            ))
        {
            LH_ERROR(
                "[PlayerDeathThink] mprotect(RX) failed"
            );

            return false;
        }

        return true;
    }

    bool Restore()
    {
        if (!g_state.target)
            return false;

        if (!SetPageProtection(
                g_state.target,
                PATCH_SIZE,
                PROT_READ | PROT_WRITE | PROT_EXEC
            ))
        {
            LH_ERROR(
                "[PlayerDeathThink] cannot make target writable"
            );

            return false;
        }

        std::memcpy(
            reinterpret_cast<void*>(g_state.target),
            g_state.original,
            PATCH_SIZE
        );

        __builtin___clear_cache(
            reinterpret_cast<char*>(g_state.target),
            reinterpret_cast<char*>(g_state.target) +
                PATCH_SIZE
        );

        if (!SetPageProtection(
                g_state.target,
                PATCH_SIZE,
                PROT_READ | PROT_EXEC
            ))
        {
            LH_ERROR(
                "[PlayerDeathThink] cannot restore RX permissions"
            );

            return false;
        }

        return true;
    }
}

namespace PlayerDeathThinkHook
{
    bool Install()
    {
        if (g_state.installed)
        {
            LH_WARN(
                "[PlayerDeathThink] Already installed"
            );

            return true;
        }

        /*
         * Find the runtime load address of cs.so.
         */
        g_state.cs_base = GetModuleBase();

        if (!g_state.cs_base)
        {
            LH_ERROR(
                "[PlayerDeathThink] Cannot find cs.so"
            );

            return false;
        }

        g_state.target =
            g_state.cs_base +
            PLAYER_DEATH_THINK_RVA;

        LH_DEBUG(
            "[PlayerDeathThink] cs.so base: %p",
            reinterpret_cast<void*>(g_state.cs_base)
        );

        LH_DEBUG(
            "[PlayerDeathThink] target: %p",
            reinterpret_cast<void*>(g_state.target)
        );

        /*
         * Save the original first 10 bytes.
         */
        std::memcpy(
            g_state.original,
            reinterpret_cast<void*>(g_state.target),
            PATCH_SIZE
        );

        LH_DEBUG(
            "[PlayerDeathThink] Original bytes: "
            "%02X %02X %02X %02X %02X "
            "%02X %02X %02X %02X %02X",
            g_state.original[0],
            g_state.original[1],
            g_state.original[2],
            g_state.original[3],
            g_state.original[4],
            g_state.original[5],
            g_state.original[6],
            g_state.original[7],
            g_state.original[8],
            g_state.original[9]
        );

        /*
         * Verify the expected prologue before modifying executable
         * memory. This protects against silently patching the wrong
         * function if cs.so changes.
         */
        if (g_state.original[0] != 0xe8 ||
            g_state.original[5] != 0x05)
        {
            LH_ERROR(
                "[PlayerDeathThink] Unexpected function prologue"
            );

            LH_ERROR(
                "[PlayerDeathThink] Expected "
                "E8 ........ 05 ........"
            );

            return false;
        }

        if (!CreateTrampoline())
        {
            LH_ERROR(
                "[PlayerDeathThink] Cannot create trampoline"
            );

            return false;
        }

        if (!Patch())
        {
            LH_ERROR(
                "[PlayerDeathThink] Cannot patch target"
            );

            DestroyTrampoline();

            return false;
        }

        g_state.installed = true;

        LH_DEBUG(
            "[PlayerDeathThink] Hook installed"
        );

        LH_DEBUG(
            "[PlayerDeathThink] trampoline: %p",
            g_state.trampoline
        );

        return true;
    }

    void Uninstall()
    {
        if (!g_state.installed)
            return;

        if (Restore())
        {
            DestroyTrampoline();

            g_state.target = 0;
            g_state.cs_base = 0;
            g_state.installed = false;

            std::memset(
                g_state.original,
                0,
                sizeof(g_state.original)
            );

            LH_DEBUG(
                "[PlayerDeathThink] Hook removed"
            );
        }
    }

    bool IsInstalled()
    {
        return g_state.installed;
    }
}
