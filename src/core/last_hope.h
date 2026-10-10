#pragma once
#include "player/player_team.h"
#include "player/player_state.h"

extern bool can_last_hope_use;

extern int g_last_hope_player_id;

extern bool g_last_hope_pending;

extern SavedWeapon g_last_hope_weapon;

bool LastHope_CheckWinCondition(TeamStatus& t, TeamStatus& ct);
bool LastHope_SetRandomPlayer(int& last_hope_player_id, const TeamStatus& t, const TeamStatus& ct);
bool LastHope_TryRespawnPlayer(int last_hope_player_id);

bool LastHope_SaveWeapon(void* player);
bool LastHope_RestoreWeapon(void* player, const SavedWeapon& saved);

void setCanLastHopeUse(bool last_hope);
bool getCanLastHopeUse();
