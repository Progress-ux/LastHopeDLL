#include "last_hope.h"

#include <dllapi.h>

#include "game_rules/last_hope_rules.h"
#include "player/player_methods.h"
#include "util/logger.h"
#include "sdk_util.h"

bool last_hope_used = false;
int LAST_HOPE_CHANCE = 20;

bool LastHope_TryRespawnPlayer()
{
    LH_DEBUG("last_hope_used=%d", last_hope_used);
    if (last_hope_used)
    {
        LH_DEBUG("last_hope_used");
        return false;
    }

    // if (RANDOM_LONG(1, 100) > LAST_HOPE_CHANCE)
    //     return;

    TeamStatus t = GetTeamStatus(PlayerTeam::TEAM_TERRORIST);
    TeamStatus ct = GetTeamStatus(PlayerTeam::TEAM_CT);
    LH_DEBUG("T: alive=%d dead=%d | CT: alive=%d dead=%d",
         t.alive, t.dead, ct.alive, ct.dead);
    if (!IsLastHopeSituation(t, ct))
    {
        LH_DEBUG(
            "!IsLastHopeSituation()"
        );
        return false;
    }

    PlayerTeam team_last_hope = GetLastHopeTeam(t, ct);
    LH_DEBUG("team_last_hope=%d", static_cast<int>(team_last_hope));

    int last_hope_player_id = FindLastHopePlayer(team_last_hope);
    if (!last_hope_player_id)
    {
        LH_DEBUG(
            "!last_hope_player_id"
        );
        return false;
    }

    edict_t* ent = INDEXENT(last_hope_player_id);

    LH_INFO(
        "[LastHope] [Respawn] Candidate: name=\"%s\" id=%d",
        STRING(ent->v.netname),
        last_hope_player_id
    );

    PlayerMethods::RoundRespawn(ent);

    ent->v.deadflag = DEAD_NO;
    ent->v.health = 100;              // или сколько нужно
    ent->v.takedamage = DAMAGE_YES;
    ent->v.solid = SOLID_SLIDEBOX;
    ent->v.movetype = MOVETYPE_WALK;
    ent->v.flags &= ~FL_ONGROUND;
    ent->v.velocity = Vector(0, 0, 0);
    ent->v.weapons = 0;               // сброс оружия

    ent->v.iuser1 = 0;
    ent->v.iuser2 = 0;
    ent->v.iuser3 = 0;

    last_hope_used = true;
    return true;
}

void setLastHopeUsed(bool last_hope) { last_hope_used = last_hope; }
bool getLastHopeUsed() { return last_hope_used; }
