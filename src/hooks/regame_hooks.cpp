#include "regame_hooks.h"

#include "abi/regamehookchain_abi.h"
#include "core/last_hope.h"
#include "player/player_state.h"
#include "util/logger.h"
#include "regame_context.h"
#include <dllapi.h>
#include "sdk_util.h"

namespace
{
    regame::IReGameHookRegistry_CSGameRules_CheckWinConditions*
        g_checkWinConditions = nullptr;
    regame::IReGameHookRegistry_CSGameRules_RestartRound*
        g_restartRound = nullptr;
    regame::IReGameHookRegistry_CBasePlayer_Killed*
        g_playerKilled = nullptr;

    int GetPlayerId(CBasePlayer* player)
    {
        if (!player)
            return 0;

        for (int i = 1; i <= MAX_PLAYERS; ++i)
        {
            edict_t* ent = g_engfuncs.pfnPEntityOfEntIndex(i);

            if (ent && !ent->free && ent->pvPrivateData == player)
                return i;
        }
        return 0;
    }

    void OnCheckWinConditions(regame::IHookChain<void>* chain)
    {
        TeamStatus t{}, ct{};
        if (LastHope_CheckWinCondition(t, ct))
            return; 

        chain->callNext();
    }

    void OnRestartRound(regame::IHookChain<void>* chain)
    {
        setCanLastHopeUse(true);
        g_last_hope_player_id = -1;
        g_last_hope_pending = false;
        LH_DEBUG("RestartRound, can_last_hope_use=%d", getCanLastHopeUse());
        chain->callNext();
    }

    void OnPlayerKilled(
        regame::CBasePlayer_KilledChain* chain,
        CBasePlayer* player,
        entvars_s* attacker,
        int gib
    )
    {
        const int playerId = GetPlayerId(player);

        LH_DEBUG("player=%p playerId=%d", player, playerId);

        if (playerId >= 1 && playerId <= MAX_PLAYERS)
            g_players[playerId].SavePlayerInventory(player, playerId);

        chain->callNext(player, attacker, gib);
    }
} // namespace



bool RegisterCheckWinConditionsHook()
{
    auto* hookchains = ReGameContext::Hookchains();
    if (!hookchains)
    {
        LH_ERROR("hookchains is nullptr!");
        return false;
    }

    g_checkWinConditions = hookchains->CSGameRules_CheckWinConditions();
    if (!g_checkWinConditions)
    {
        LH_ERROR("registry is nullptr!");
        return false;
    }

    g_checkWinConditions->registerHook(
        OnCheckWinConditions,
        regame::HC_PRIORITY_DEFAULT
    );

    LH_INFO("hook registered!");

    return true;
}

bool RegisterRestartRoundHook()
{
    auto* hookchains = ReGameContext::Hookchains();
    if (!hookchains)
    {
        LH_ERROR("hookchains is nullptr!");
        return false;
    }

    g_restartRound =
        hookchains->CSGameRules_RestartRound();

    if (!g_restartRound)
    {
        LH_ERROR("registry is nullptr!");
        return false;
    }

    g_restartRound->registerHook(
        OnRestartRound,
        regame::HC_PRIORITY_DEFAULT
    );

    LH_INFO("hook registered!");

    return true;
}

bool RegisterPlayerKilledHook()
{
    auto* hookchains = ReGameContext::Hookchains();
    if (!hookchains)
    {
        LH_ERROR("hookchains is nullptr!");
        return false;
    }

    g_playerKilled =
        hookchains->CBasePlayer_Killed();

    if (!g_playerKilled)
    {
        LH_ERROR("registry is nullptr!");
        return false;
    }

    g_playerKilled->registerHook(
        OnPlayerKilled,
        regame::HC_PRIORITY_DEFAULT
    );

    LH_INFO("hook registered!");

    return true;

}

void UnregisterCheckWinConditionsHook()
{
    if (!g_checkWinConditions)
        return;

    g_checkWinConditions->unregisterHook(OnCheckWinConditions);

    g_checkWinConditions = nullptr;

    LH_INFO("hook unregistered");
}

void UnregisterRestartRoundHook()
{
    if (!g_restartRound)
        return;

    g_restartRound->unregisterHook(OnRestartRound);

    g_restartRound = nullptr;

    LH_INFO("hook unregistered");
}

void UnregisterPlayerKilledHook()
{
    if (!g_playerKilled)
        return;

    g_playerKilled->unregisterHook(OnPlayerKilled);

    g_playerKilled = nullptr;

    LH_INFO("hook unregistered");
}
