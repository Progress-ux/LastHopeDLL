#include "last_hope.h"

#include <cstdint>
#include <dlfcn.h>
#include <dllapi.h>

#include "const.h"
#include "enginecallback.h"
#include "player_state.h"
#include "player/player_team.h"
#include "logger.h"
#include "sdk_util.h"

qboolean LastHope_ClientConnect(
    edict_t *pEntity, 
    const char *pszName, 
    const char *pszAddress, 
    char *szRejectReason
)
{
    int index = ENTINDEX(pEntity);

    if (index < 1 || index > MAX_PLAYERS)
        return TRUE;

    g_players[index] = {};

    bool alive = (pEntity->v.deadflag == DEAD_NO);

    PlayerState& player = g_players[index];
    player.playerConnected(alive);

    LH_INFO(
        "ClientConnect: id=%d name=\"%s\" address=\"%s\"",
        index,
        pszName,
        pszAddress
    );

    return TRUE;
}

void LastHope_ClientPutInServer(edict_t *pEntity)
{
    int index = ENTINDEX(pEntity);

    if (index < 1 || index > MAX_PLAYERS)
        return;

    bool alive = (pEntity->v.deadflag == DEAD_NO);

    PlayerState& player = g_players[index];
    player.playerPutInServer(alive);

    LH_INFO(
        "ClientPutInServer: id=%d name=\"%s\"",
        index,
        STRING(pEntity->v.netname)
    );
}

void LastHope_ClientDisconnect(edict_t *pEntity)
{
    int index = ENTINDEX(pEntity);

    if (index < 1 || index > MAX_PLAYERS)
        return;

    g_players[index] = {};

    LH_INFO(
        "ClientDisconnect: id=%d name=\"%s\"",
        index,
        STRING(pEntity->v.netname)
    );
}

void LastHope_PlayerPostThink(edict_t* pEntity)
{
    int index = ENTINDEX(pEntity);

    if (index < 1 || index > MAX_PLAYERS)
        return;

    PlayerState& player = g_players[index];

    if (!pEntity->pvPrivateData)
        return;

    bool alive = (pEntity->v.deadflag == DEAD_NO);

    if (!player.connected)
    {
        player.playerConnected(alive);

        LH_INFO(
            "[PostThink] Registered player: id=%d name=\"%s\" alive=%d",
            index,
            STRING(pEntity->v.netname),
            alive
        );

        return;
    }

    if (!player.initialized)
    {
        player.wasAlive = alive;
        player.initialized = true;
        return;
    }

    if (player.wasAlive && !alive)
    {
        Vector origin = pEntity->v.origin;

        player.deathOrigin[0] = origin.x;
        player.deathOrigin[1] = origin.y;
        player.deathOrigin[2] = origin.z;

        LH_DEBUG(
            "PlayerDeath: id=%d name=\"%s\" origin=(%.1f %.1f %.1f)",
            index,
            STRING(pEntity->v.netname),
            origin.x,
            origin.y,
            origin.z
        );
    }

    player.wasAlive = alive;
}

void LastHope_CheckWinCondition()
{
    if (last_hope_used)
        return;

    if (RANDOM_LONG(1, 100) > LAST_HOPE_CHANCE)
        return;

    TeamStatus t = GetTeamStatus(PlayerTeam::TEAM_TERRORIST);
    TeamStatus ct = GetTeamStatus(PlayerTeam::TEAM_CT);

    if (!IsLastHopeSituation(t, ct))
        return;

    PlayerTeam team_last_hope = GetLastHopeTeam(t, ct);

    int last_hope_player_id = FindLastHopePlayer(team_last_hope);
    if (!last_hope_player_id)
        return;

    edict_t* ent = INDEXENT(last_hope_player_id);

    LH_INFO(
        "[LastHope] [Respawn] Candidate: name=\"%s\" id=%d",
        STRING(ent->v.netname),
        last_hope_player_id
    );

    last_hope_used = true;
}

bool IsLastHopeSituation(const TeamStatus &t, const TeamStatus &ct)
{
    const bool tEliminated = 
        t.alive == 0 && t.dead > 0;

    const bool ctEliminated = 
        ct.alive == 0 && ct.dead > 0;

    return tEliminated != ctEliminated;
}

PlayerTeam GetLastHopeTeam(const TeamStatus &t, const TeamStatus &ct)
{
    if (t.alive == 0 && t.dead > 0 && ct.alive > 0)
        return TEAM_TERRORIST;

    if (ct.alive == 0 && ct.dead > 0 && t.alive > 0)
        return TEAM_CT;

    return TEAM_UNASSIGNED;
}

int FindLastHopePlayer(PlayerTeam team)
{
    int canditates[MAX_PLAYERS];
    int count = 0;

    for (int id = 1; id <= MAX_PLAYERS; ++id)
    {
        PlayerState& player = g_players[id];

        if (!player.connected || !player.in_game)
            continue;

        edict_t* ent = INDEXENT(id);

        if (!ent || ent->free)
            continue;

        if (GetPlayerTeam(ent) != team)
            continue;

        if (ent->v.deadflag == DEAD_NO)
            continue;

        canditates[count++] = id;
    }

    if (count == 0)
        return 0;

    return canditates[RANDOM_LONG(0, count - 1)];
}
