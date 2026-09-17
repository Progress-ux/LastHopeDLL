#include "regame_context.h"

#include "util/logger.h"

namespace 
{
    regame::IReGameApi*         g_api        = nullptr;
    regame::IReGameHookchains*  g_hookchains = nullptr;
}

namespace ReGameContext
{
    bool Init(regame::IReGameApi *api)
    {
        if (!api)
        {
            LH_ERROR("[ReGameContext] Init: api == nullptr");
            return false;
        }

        if (g_api)
        {
            LH_WARN("[ReGameContext] Init: already initialized");
            return false;
        }

        g_api = api;
        g_hookchains = api->GetHookchains();

        if (!g_hookchains)
        {
            LH_ERROR("[ReGameContext] Init: GetHookchains() returned nullptr");
            return false;
        }

        LH_INFO(
            "[ReGameContext] Init: api=%p hookchains=%p version=%d.%d",
            static_cast<void*>(g_api),
            static_cast<void*>(g_hookchains),
            api->GetMajorVersion(),
            api->GetMinorVersion()
        );

        return true;
    }

    void Reset()
    {
        g_api = nullptr;
        g_hookchains = nullptr;
        LH_INFO("[ReGameContext] Reset");
    }

    bool isInitialize() { return g_api != nullptr && g_hookchains != nullptr; } 
    regame::IReGameApi* Api() { return g_api; }
    regame::IReGameHookchains* Hookchains() { return g_hookchains; }

} // namespace ReGameContext
