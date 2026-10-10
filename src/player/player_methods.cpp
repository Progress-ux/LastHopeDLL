#include "player_methods.h"

#include "const.h"
#include "hooks/player_deaththink_hook.h"
#include "util/logger.h"
#include <cstdint>
#include <cstring>

#define SVC_SETVIEW 5

namespace 
{
    constexpr std::size_t ROUND_RESPAWN_VTABLE_INDEX = 84;
    constexpr uintptr_t GIVE_NAMED_ITEM_RVA = 0x14b040;
    constexpr std::uintptr_t GIVE_NAMED_ITEM_OFFSET = 0x13dc60;

    using RoundRespawnFn  = void (*)(void*);
    using GiveNamedItemOrigFn = void* (*)(void* player, const char* classname);
}

namespace PlayerMethods
{
    void RoundRespawn(edict_t *ent)
    {
        if (!ent || !ent->pvPrivateData)
        {
            LH_ERROR(
                "Invalid player/private data"
            );
            return;
        }

        void* player = ent->pvPrivateData;
        void** vtable = *reinterpret_cast<void***>(player);

        auto fn = reinterpret_cast<RoundRespawnFn>(
            vtable[ROUND_RESPAWN_VTABLE_INDEX]
        );
        fn(player);
    }

    void* GiveNamedItem(
        CBasePlayer* player,
        const char* name
    )
    {
        if (!player || !name || !g_state.cs_base)
            return nullptr;

        auto giveNamedItemOrig = reinterpret_cast<GiveNamedItemOrigFn>(
            g_state.cs_base + GIVE_NAMED_ITEM_OFFSET
        );

        LH_DEBUG(
            "[GiveNamedItem] before: player=%p name=%s base=%p",
            static_cast<void*>(player),
            name,
            reinterpret_cast<void*>(g_state.cs_base)
        );

        void* item = giveNamedItemOrig(player, name);

        LH_DEBUG(
            "[GiveNamedItem] after: player=%p name=%s result=%p",
            static_cast<void*>(player),
            name,
            item
        );

        return item; 
    }
}
