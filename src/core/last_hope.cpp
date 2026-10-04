#include "last_hope.h"

#include <dllapi.h>

#include "game_rules/last_hope_rules.h"
#include "player/player_methods.h"
#include "player/player_team.h"
#include "util/logger.h"
#include "sdk_util.h"

bool can_last_hope_use = false;
int  g_last_hope_player_id = -1;
bool g_last_hope_pending = false;
int  LAST_HOPE_CHANCE = 20;

bool LastHope_CheckWinCondition(TeamStatus& t, TeamStatus& ct)
{
    LH_DEBUG("last_hope_used=%d", can_last_hope_use);
    if (!can_last_hope_use)
    {
        LH_DEBUG("last_hope already used");
        return false;
    }

    t  = GetTeamStatus(PlayerTeam::TEAM_TERRORIST);
    ct = GetTeamStatus(PlayerTeam::TEAM_CT);

    LH_DEBUG("T: alive=%d dead=%d | CT: alive=%d dead=%d",
             t.alive, t.dead, ct.alive, ct.dead);

    if (!IsLastHopeSituation(t, ct))
    {
        LH_DEBUG("!IsLastHopeSituation()");
        return false;
    }

    if (RANDOM_LONG(1, 100) > LAST_HOPE_CHANCE)
    {
        LH_DEBUG("LastHope chance failed");
        return false;
    }

    // выбираем игрока
    int candidate = 0;
    if (!LastHope_SetRandomPlayer(candidate, t, ct))
    {
        LH_DEBUG("!LastHope_SetRandomPlayer()");
        return false;
    }

    g_last_hope_player_id = candidate;
    g_last_hope_pending = true;
    can_last_hope_use = false; 

    LH_DEBUG("[LastHope] Activated for player id=%d", g_last_hope_player_id);
    return true;
}

bool LastHope_SetRandomPlayer(int& last_hope_player_id, const TeamStatus& t, const TeamStatus& ct)
{
    PlayerTeam team_last_hope = GetLastHopeTeam(t, ct);
    LH_DEBUG("team_last_hope=%d", static_cast<int>(team_last_hope));

    last_hope_player_id = FindLastHopePlayer(team_last_hope);
    if (!last_hope_player_id)
    {
        LH_DEBUG("!last_hope_player_id");
        return false;
    }
    return true;
}

bool LastHope_TryRespawnPlayer(int last_hope_player_id)
{
    if (last_hope_player_id <= 0)
        return false;

    edict_t* ent = INDEXENT(last_hope_player_id);
    if (!ent || ent->free)
        return false;

    LH_DEBUG("[LastHope] [Respawn] Candidate: name=\"%s\" id=%d",
            STRING(ent->v.netname), last_hope_player_id);

    PlayerMethods::RoundRespawn(ent);

    ent->v.deadflag   = DEAD_NO;
    ent->v.health     = 100;
    ent->v.takedamage = DAMAGE_YES;
    ent->v.solid      = SOLID_SLIDEBOX;
    ent->v.movetype   = MOVETYPE_WALK;
    ent->v.flags     &= ~FL_ONGROUND;
    ent->v.velocity   = Vector(0, 0, 0);
    ent->v.weapons    = 0;

    ent->v.iuser1 = 0;
    ent->v.iuser2 = 0;
    ent->v.iuser3 = 0;

    return true;
}

void setLastHopeUsed(bool last_hope) { can_last_hope_use = last_hope; }
bool getLastHopeUsed() { return can_last_hope_use; }
