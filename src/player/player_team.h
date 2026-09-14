#pragma once

#include <extdll.h>

#include <cstring>

enum PlayerTeam
{
    TEAM_UNASSIGNED = 0,
    TEAM_TERRORIST  = 1,
    TEAM_CT         = 2,
    TEAM_SPECTATOR  = 3
};

struct TeamStatus 
{
    int alive = 0;
    int dead = 0;
};

constexpr std::size_t TEAM_OFFSET = 476;
PlayerTeam GetPlayerTeam(edict_t *pEntity);
TeamStatus GetTeamStatus(PlayerTeam team);
