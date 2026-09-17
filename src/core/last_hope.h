#pragma once 

#include <extdll.h>

extern bool last_hope_used;
extern int LAST_HOPE_CHANCE;

void LastHope_PlayerKilled(
    edict_t* pVictim,
    edict_t* pKiller
);

void LastHope_CheckWinCondition();

void setLastHopeUsed(bool last_hope);
bool getLastHopeUsed();
