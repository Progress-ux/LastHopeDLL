#include "regame_hooks.h"

#include "core/last_hope.h"
#include "util/logger.h"
#include "regame_context.h"
#include <dllapi.h>

namespace 
{

    regame::IReGameHookRegistry_CSGameRules_CheckWinConditions* 
        g_checkWinConditions = nullptr;
    regame::IReGameHookRegistry_CSGameRules_RestartRound* 
        g_restartRound = nullptr;

    void OnCheckWinConditions(regame::IHookChain<void>* chain)
    {
        LastHope_CheckWinCondition();
        chain->callNext();
    }

    void OnRestartRound(regame::IHookChain<void>* chain)
    {
        setLastHopeUsed(false);
        LH_DEBUG("RestartRound, last_hope_used=%d", getLastHopeUsed());
        chain->callNext();
    }

} // namespace



bool RegisterCheckWinConditionsHook()
{
    auto* hookchains = ReGameContext::Hookchains();
    if (!hookchains)
    {
        LH_ERROR("[RegisterCheckWinConditionsHook] hookchains is nullptr!");
        return false;
    }

    g_checkWinConditions = hookchains->CSGameRules_CheckWinConditions();
    if (!g_checkWinConditions)
    {
        LH_ERROR("[RegisterCheckWinConditionsHook] registry is nullptr!");
        return false;
    }

    g_checkWinConditions->registerHook(
        OnCheckWinConditions,
        regame::HC_PRIORITY_DEFAULT
    );

    LH_INFO("[RegisterCheckWinConditionsHook] hook registered!");

    return true;
}

bool RegisterRestartRoundHook()
{
    auto* hookchains = ReGameContext::Hookchains();
    if (!hookchains)
    {
        LH_ERROR("[RegisterRestartRoundHook] hookchains is nullptr!");
        return false;
    }

    g_restartRound =
        hookchains->CSGameRules_RestartRound();

    if (!g_restartRound)
    {
        LH_ERROR("[RegisterRestartRoundHook] registry is nullptr!");
        return false;
    }

    g_restartRound->registerHook(
        OnRestartRound,
        regame::HC_PRIORITY_DEFAULT
    );

    LH_INFO("[RegisterRestartRoundHook] hook registered!");

    return true;
}

void UnregisterCheckWinConditionsHook()
{
    if (!g_checkWinConditions)
        return;

    g_checkWinConditions->unregisterHook(OnCheckWinConditions);

    g_checkWinConditions = nullptr;

    LH_INFO("[UnregisterCheckWinConditionsHook] hook unregistered");
}

void UnregisterRestartRoundHook()
{
    if (!g_restartRound)
        return;

    g_restartRound->unregisterHook(OnRestartRound);

    g_restartRound = nullptr;

    LH_INFO("[UnregisterRestartRoundHook] hook unregistered");
}
