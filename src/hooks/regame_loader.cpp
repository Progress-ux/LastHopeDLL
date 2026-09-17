#include <extdll.h>

#include "regame_loader.h"

#include <dlfcn.h>
#include <link.h>

#include <cstring>

#include "hooks/regame_hooks.h"
#include "hooks/regame_context.h"
#include "util/logger.h"

namespace 
{
    const char* g_game_dll_path = nullptr;

    int FindGameDLL( struct dl_phdr_info* info, size_t, void*)
    {
        if (!info->dlpi_name || !info->dlpi_name[0])
            return 0;

        if (std::strstr(info->dlpi_name, "/cstrike/dlls/cs.so"))
        {
            g_game_dll_path = info->dlpi_name;
            return 1;
        }

        return 0;
    }

    regame::IReGameApi* LoadReGameApi()
    {
        dl_iterate_phdr(FindGameDLL, nullptr);
        
        if (!g_game_dll_path)
        {
            LH_ERROR("[LoadReGameApi] GameDLL not found!");
            return nullptr;
        }

        LH_DEBUG("[LoadReGameApi] GameDLL path: \"%s\"", g_game_dll_path);

        void* handle = dlopen(
            g_game_dll_path,
            RTLD_NOW | RTLD_NOLOAD
        );

        if (!handle)
        {
            LH_ERROR(
                "[LoadReGameApi] Failed to get GameDLL handle: %s",
                dlerror()
            );
            return nullptr;
        }

        using CreateInterfaceFn = void* (*)(const char*, int*);

        auto factory =
            reinterpret_cast<CreateInterfaceFn>(
                dlsym(handle, "CreateInterface")
            );

        if (!factory)
        {
            LH_ERROR(
                "[LoadReGameApi] Symbol \"CreateInterface\" not found!"
            );
            return nullptr;
        }

        LH_DEBUG(
            "[LoadReGameApi] CreateInterface found: %p",
            reinterpret_cast<void*>(factory)
        );

        int returnCode = 0;

        void* api = factory(
            "VRE_GAMEDLL_API_VERSION001",
            &returnCode
        );

        if (!api)
        {
            LH_ERROR(
                "[LoadReGameApi] CreateInterface returned nullptr! "
                "returnCode=%d",
                returnCode
            );
            return nullptr;
        }

        return static_cast<regame::IReGameApi*>(api);
    }
} // namespace


bool Initialize()
{
    if (ReGameContext::isInitialize())
    {
        LH_WARN("[Initialize] Already initialized");
        return true;
    }

    regame::IReGameApi* api = LoadReGameApi();
    if (!api)
        return false;

    if (!ReGameContext::Init(api))
        return false;

    if (!RegisterCheckWinConditionsHook())
        return false;

    if (!RegisterRestartRoundHook())
        return false;

    return true;
}
