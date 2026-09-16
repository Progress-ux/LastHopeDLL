#pragma once 

#include "player/player_team.h"
#include <extdll.h>

static bool last_hope_used = false;

static int LAST_HOPE_CHANCE = 20;

qboolean LastHope_ClientConnect(
    edict_t* pEntity,
    const char* pszName,
    const char* pszAddress,
    char szRejectReason[128]
);
void LastHope_ClientPutInServer(edict_t *pEntity);
void LastHope_ClientDisconnect(edict_t *pEntity);

void LastHope_PlayerPostThink(edict_t* pEntity);

void LastHope_PlayerKilled(
    edict_t* pVictim,
    edict_t* pKiller
);

void LastHope_CheckWinCondition();

bool IsLastHopeSituation(const TeamStatus& t, const TeamStatus& ct);
PlayerTeam GetLastHopeTeam(const TeamStatus& t, const TeamStatus& ct);
int FindLastHopePlayer(PlayerTeam team);
