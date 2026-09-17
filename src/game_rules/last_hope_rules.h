#pragma once 

#include "player/player_team.h"

bool IsLastHopeSituation(const TeamStatus& t, const TeamStatus& ct);
PlayerTeam GetLastHopeTeam(const TeamStatus& t, const TeamStatus& ct);
int FindLastHopePlayer(PlayerTeam team);
