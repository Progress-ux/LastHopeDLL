#include "last_hope.h"

#include <dllapi.h>

#include "player_state.h"
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
        player.deathOrigin[2] = origin.z;

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

    LH_INFO(
        "PlayerKilled: id=%d name=\"%s\" killed=%d origin=(%.1f %.1f %.1f)",
        index,
        STRING(pVictim->v.netname),
        ENTINDEX(pKiller),
        origin.x,
        origin.y,
        origin.z
    );
}
