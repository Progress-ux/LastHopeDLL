#include "player_team.h"

#include <cstdint>

#include "player_state.h"
#include "sdk_util.h"

PlayerTeam GetPlayerTeam(edict_t* pEntity)
{
    if (!pEntity)
        return TEAM_UNASSIGNED;

    if (!pEntity->pvPrivateData)
    {
        return TEAM_UNASSIGNED;
    }

    std::int32_t team = 0;

    const auto* data =
        static_cast<const std::uint8_t*>(pEntity->pvPrivateData);

    std::memcpy(
        &team,
        data + TEAM_OFFSET,
        sizeof(team)
    );

    if (team < TEAM_UNASSIGNED || team > TEAM_SPECTATOR)
        return TEAM_UNASSIGNED;

    return static_cast<PlayerTeam>(team);
}

TeamStatus GetTeamStatus(PlayerTeam team)
{
    TeamStatus status;

    for (int id = 1; id <= MAX_PLAYERS; ++id)
    {
        edict_t* ent = INDEXENT(id);

        if (!ent || ent->free)
            continue;

        PlayerTeam playerTeam = GetPlayerTeam(ent);

        if (playerTeam != team)
            continue;

        if (ent->v.deadflag == DEAD_NO)
            ++status.alive;
        else
            ++status.dead;
    }

    return status;
}
