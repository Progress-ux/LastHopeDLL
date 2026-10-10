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
            LH_ERROR("Init: api == nullptr");
            return false;
        }

        if (g_api)
        {
            LH_WARN("Init: already initialized");
            return false;
        }

        g_api = api;
        g_hookchains = api->GetHookchains();

        if (!g_hookchains)
        {
            LH_ERROR("Init: GetHookchains() returned nullptr");
            return false;
        }

        LH_INFO(
            "Init: api=%p hookchains=%p version=%d.%d",
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
        LH_INFO("Reset");
    }

    bool isInitialize() { return g_api != nullptr && g_hookchains != nullptr; } 
    regame::IReGameApi* Api() { return g_api; }
    regame::IReGameHookchains* Hookchains() { return g_hookchains; }

} // namespace ReGameContext
