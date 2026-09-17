#include <dlfcn.h>
#include <link.h>

#include <extdll.h>

#include "regame/regame_api.h"
#include "const.h"
#include "last_hope.h"
#include "logger.h"
#include "regame/regame_abi.h"
#include "regame/regamehookchain_abi.h"
#include "sdk_util.h"


static const char* g_game_dll_path = nullptr;
static regame::IReGameApi* g_regame_api = nullptr;
static regame::IReGameHookchains* g_hookchains = nullptr;
static regame::IReGameHookRegistry_CSGameRules_CheckWinConditions* 
    g_checkWinConditions = nullptr;
static regame::IReGameHookRegistry_CSGameRules_RestartRound* 
    g_restartRound = nullptr;

static int FindGameDLL(
    struct dl_phdr_info* info,
    size_t,
    void*
)
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

void OnCheckWinConditions(
    regame::IHookChain<void>* chain
)
{
    LastHope_CheckWinCondition();
    chain->callNext();
}

bool RegisterCheckWinConditionsHook()
{
    if (!g_hookchains)
    {
        LH_ERROR("[RegisterCheckWinConditionsHook()] hookchains is nullptr!");
        return false;
    }

    g_checkWinConditions = 
        g_hookchains->CSGameRules_CheckWinConditions();

    if (!g_checkWinConditions)
    {
        LH_ERROR("[RegisterCheckWinConditionsHook()] registry is nullptr!");
        return false;
    }

    g_checkWinConditions->registerHook(
        OnCheckWinConditions,
        regame::HC_PRIORITY_DEFAULT
    );

    LH_INFO(
        "[RegisterCheckWinConditionsHook()] CheckWinConditions hook registered!"
    );

    return true;
}

bool Initialize()
{
    dl_iterate_phdr(FindGameDLL, nullptr);

    if (!g_game_dll_path)
    {
        LH_ERROR("[Initialize()] GameDLL not found!");
        return false;
    }

    LH_DEBUG(
        "[Initialize()] GameDLL path: \"%s\"",
        g_game_dll_path
    );

    void* handle = dlopen(
        g_game_dll_path,
        RTLD_NOW | RTLD_NOLOAD
    );

    if (!handle)
    {
        LH_ERROR(
            "[Initialize()] Failed to get GameDLL handle: %s",
            dlerror()
        );
        return false;
    }

    using CreateInterfaceFn = void* (*)(const char*, int*);

    auto factory =
        reinterpret_cast<CreateInterfaceFn>(
            dlsym(handle, "CreateInterface")
        );

    if (!factory)
    {
        LH_ERROR(
            "[Initialize()] Symbol \"CreateInterface\" not found!"
        );
        return false;
    }

    LH_DEBUG(
        "[Initialize()] CreateInterface found: %p",
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
            "[Initialize()] CreateInterface returned nullptr! "
            "returnCode=%d",
            returnCode
        );
        return false;
    }

    g_regame_api = static_cast<regame::IReGameApi*>(api);

    LH_INFO(
        "[Initialize()] IReGameApi acquired: %p",
        static_cast<void*>(g_regame_api)
    );

    int major = g_regame_api->GetMajorVersion();
    int minor = g_regame_api->GetMinorVersion();

    LH_INFO(
        "[Initialize()] ReGameDll version: %d.%d",
        major,
        minor
    );

    g_hookchains = g_regame_api->GetHookchains();
        
    if (!RegisterCheckWinConditionsHook())
        return false;

    if (!RegisterRestartRoundHook())
        return false;

    return true;
}

void OnRestartRound(
    regame::IHookChain<void>* chain
)
{
    setLastHopeUsed(false);
    LH_DEBUG("RestartRound, last_hope_used=%d", getLastHopeUsed());
    chain->callNext();
}

bool RegisterRestartRoundHook()
{
    if (!g_hookchains)
    {
        LH_ERROR("[RegisterRestartRoundHook()] hookchains is nullptr!");
        return false;
    }

    g_restartRound =
        g_hookchains->CSGameRules_RestartRound();

    if (!g_restartRound)
    {
        LH_ERROR("[RegisterRestartRoundHook()] registry is nullptr!");
        return false;
    }

    g_restartRound->registerHook(
        OnRestartRound,
        regame::HC_PRIORITY_DEFAULT
    );

    LH_INFO(
        "[RegisterRestartRoundHook()] RestartRound hook registered!"
    );

    return true;
}
