#include "last_hope.h"

#include <dllapi.h>

#include "const.h"
#include "game_rules/last_hope_rules.h"
#include "player/player_methods.h"
#include "player/player_state.h"
#include "player/player_team.h"
#include "util/logger.h"
#include "sdk_util.h"
#include "util/weapon.h"

bool can_last_hope_use = false;
int  g_last_hope_player_id = -1;
bool g_last_hope_pending = false;
int  LAST_HOPE_CHANCE = 20;

namespace 
{
    void RestoreSavedWeapon(CBasePlayer* player, int playerId)
    {
        if (!player || playerId < 1 || playerId > MAX_PLAYERS)
            return;

        auto& inventory = g_players[playerId].inventory;

        if (!inventory.valid)
        {
            return;
        }

        for(int i = 0; i < inventory.weaponCount; ++i)
        {
            const SavedWeapon& saved = inventory.weapons[i];

            if (!saved.valid)
                continue;

            const char* classname = WeaponIdToClassname(saved.id);

            if (!classname)
            {
                continue;
            }

            LH_DEBUG(
                "before GiveNamedItem: player=%p id=%d classname=%s",
                player, saved.id, classname
            );

            auto* item = PlayerMethods::GiveNamedItem(player, classname);

            LH_DEBUG(
                "after GiveNamedItem: item=%p",
                item
            );

            LH_DEBUG(
                "player=%d weapon=%s item=%p",
                playerId, classname, item
            );
        }
    }
}

bool LastHope_CheckWinCondition(TeamStatus& t, TeamStatus& ct)
{
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

    // if (RANDOM_LONG(1, 100) > LAST_HOPE_CHANCE)
    // {
    //     LH_DEBUG("LastHope chance failed");
    //     return false;
    // }

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

    LH_DEBUG("Candidate: name=\"%s\" id=%d",
            STRING(ent->v.netname), last_hope_player_id);

    PlayerMethods::RoundRespawn(ent);

    ent->v.deadflag   = DEAD_NO;
    ent->v.health     = 100;
    ent->v.takedamage = DAMAGE_YES;
    ent->v.solid      = SOLID_SLIDEBOX;
    ent->v.movetype   = MOVETYPE_WALK;
    ent->v.flags     &= ~FL_ONGROUND;
    ent->v.velocity   = Vector(0, 0, 0);

    void* privateData = ent->pvPrivateData;

    LH_DEBUG(
        "[Respawn] ent=%p pvPrivateData=%p playerId=%d inventoryValid=%d",
        ent,
        privateData,
        last_hope_player_id,
        g_players[last_hope_player_id].inventory.valid
    );

    if (!privateData)
    {
        LH_DEBUG("[Respawn] pvPrivateData is null");
        return false;
    }

    RestoreSavedWeapon(
        reinterpret_cast<CBasePlayer*>(privateData),
        last_hope_player_id
    );

    LH_DEBUG("[Respawn] RestoreSavedWeapon returned");

    ent->v.iuser1 = 0;
    ent->v.iuser2 = 0;
    ent->v.iuser3 = 0;

    return true;
}

void setCanLastHopeUse(bool last_hope) { can_last_hope_use = last_hope; }
bool getCanLastHopeUse() { return can_last_hope_use; }
