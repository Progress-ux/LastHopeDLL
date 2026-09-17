#include "last_hope_rules.h"

#include "player/player_state.h"

#include <extdll.h>
#include <dllapi.h>

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
    int canditates[MAX_PLAYERS];
    int count = 0;

    for (int id = 1; id <= MAX_PLAYERS; ++id)
    {
        PlayerState& player = g_players[id];

        if (!player.connected || !player.in_game)
            continue;

        edict_t* ent = INDEXENT(id);

        if (!ent || ent->free)
            continue;

        if (GetPlayerTeam(ent) != team)
            continue;

        if (ent->v.deadflag == DEAD_NO)
            continue;

        canditates[count++] = id;
    }

    if (count == 0)
        return 0;

    return canditates[RANDOM_LONG(0, count - 1)];
}

