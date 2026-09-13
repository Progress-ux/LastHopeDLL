#include "player_team.h"

#include <cstdint>

PlayerTeam GetPlayerTeam(edict_t *pEntity)
{
    if (!pEntity || !pEntity->pvPrivateData)
        return TEAM_UNASSIGNED;

    std::int32_t team = 0;

    const auto *data =
        static_cast<const std::uint8_t *>(pEntity->pvPrivateData);

    std::memcpy(
        &team,
        data + TEAM_OFFSET,
        sizeof(team)
    );

    if (team < TEAM_UNASSIGNED || team > TEAM_SPECTATOR)
        return TEAM_UNASSIGNED;

    return static_cast<PlayerTeam>(team);
}
