#include "last_hope.h"

#include <dllapi.h>

#include "const.h"
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
    g_players[index].connected = true;


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

    g_players[index].connected = true;
    g_players[index].alive = true;
    g_players[index].wasAlive = true;
    g_players[index].in_game = true;

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

void LastHope_PlayerPostThink(edict_t *pEntity)
{

    int index = ENTINDEX(pEntity);

    if (index < 1 || index > MAX_PLAYERS)
        return;

    PlayerState& player = g_players[index];

    if (!player.connected)
        return;

    bool alive = (pEntity->v.deadflag == DEAD_NO);

    if (player.wasAlive && !alive)
    {
        Vector origin = pEntity->v.origin;

        player.alive = false;

        player.deathOrigin[0] = origin.x;
        player.deathOrigin[1] = origin.y;
        LH_INFO(
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

void LastHope_PlayerKilled(edict_t *pVictim, edict_t *pKiller)
{
    int index = ENTINDEX(pVictim);

    if (index < 1 || index > MAX_PLAYERS)
        return;

    PlayerState& player = g_players[index];

    if (!player.connected)
        return;

    player.alive = false;

    Vector origin = pVictim->v.origin;

    player.deathOrigin[0] = origin.x;
    player.deathOrigin[1] = origin.y;
    player.deathOrigin[2] = origin.z;

    LH_DEBUG(
        "[PlayerKilled] id=%d connected=%d in_game=%d",
        index,
        player.connected,
        player.in_game
    );
}

void LastHope_CheckWinCondition()
{
    TeamStatus t;
    TeamStatus ct;

    for (int id = 1; id <= MAX_PLAYERS; ++id)
    {
        PlayerState& player = g_players[id];

        if (!player.connected || !player.in_game)
            continue;

        edict_t* ent = INDEXENT(id);

        if (!ent || ent->free)
            continue;

        PlayerTeam team = GetPlayerTeam(ent);
        if (team == TEAM_TERRORIST)
            t = GetTeamStatus(team);
        else if (team == TEAM_CT)
            ct = GetTeamStatus(team);
    }

    LH_DEBUG(
        "[LastHope] T: alive=%d dead=%d | CT: alive=%d dead=%d",
        t.alive,
        t.dead,
        ct.alive,
        ct.dead
    );

    if (!IsLastHopeSituation(t, ct))
        return; 

    PlayerTeam lastHopeTeam = 
        GetLastHopeTeam(t, ct);

    int playerID = FindLastHopePlayer(lastHopeTeam);

    if (!playerID)
        return;

    LH_INFO(
        "[LastHope] Canditate: id=%d team%d",
        playerID,
        static_cast<int>(lastHopeTeam)
    );
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
    for (int id = 1; id <= MAX_PLAYERS; ++id)
    {
        PlayerState& player = g_players[id];

        if (!player.connected || !player.in_game)
            continue;

        if (player.alive)
            continue;

        edict_t* ent = INDEXENT(id);

        if (!ent || ent->free)
            continue;

        if (GetPlayerTeam(ent) != team)
            continue;

        return id;
    }

    return 0;
}
