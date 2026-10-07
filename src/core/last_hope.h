#pragma once
#include "player/player_team.h"

extern bool can_last_hope_use;

extern int g_last_hope_player_id;

extern bool g_last_hope_pending;

bool LastHope_CheckWinCondition(TeamStatus& t, TeamStatus& ct);
bool LastHope_SetRandomPlayer(int& last_hope_player_id, const TeamStatus& t, const TeamStatus& ct);
bool LastHope_TryRespawnPlayer(int last_hope_player_id);

void setCanLastHopeUse(bool last_hope);
bool getCanLastHopeUse();
