#include "entity_hooks.h"

#include "player/player_state.h"
#include "util/logger.h"
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

        // LH_INFO(
        //     "[PostThink] Registered player: id=%d name=\"%s\" alive=%d",
        //     index,
        //     STRING(pEntity->v.netname),
        //     alive
        // );
        //
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
