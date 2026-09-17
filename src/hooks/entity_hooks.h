#pragma once 

#include <extdll.h>

qboolean LastHope_ClientConnect(
    edict_t* pEntity,
    const char* pszName,
    const char* pszAddress,
    char szRejectReason[128]
);
void LastHope_ClientDisconnect(edict_t *pEntity);

void LastHope_ClientPutInServer(edict_t *pEntity);

void LastHope_PlayerPostThink(edict_t* pEntity);
