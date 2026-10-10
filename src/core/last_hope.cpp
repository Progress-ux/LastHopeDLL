#include "last_hope.h"

#include <dllapi.h>

#include "abi/regame_player_abi.h"
#include "const.h"
#include "game_rules/last_hope_rules.h"
#include "player/player_methods.h"
#include "player/player_state.h"
#include "player/player_team.h"
#include "util/logger.h"
#include "sdk_util.h"
#include "util/weapon.h"
#include "vector.h"

bool can_last_hope_use = false;
int  g_last_hope_player_id = -1;
bool g_last_hope_pending = false;
int  LAST_HOPE_CHANCE = 20;

namespace 
{
    void RestorePlayerOrigin(edict_t* ent, const PlayerState& player)
    {
        if (!ent || !ent->pvPrivateData)
            return;

        ent->v.origin.x = player.deathOrigin[0];
        ent->v.origin.y = player.deathOrigin[1];
        ent->v.origin.z = player.deathOrigin[2];

        ent->v.oldorigin = ent->v.origin;

        ent->v.velocity = Vector(0.0f, 0.0f, 0.0f);
        ent->v.basevelocity = Vector(0.0f, 0.0f, 0.0f);
        LH_DEBUG(
            "player=%d origin=(%.1f %.1f %.1f)",
            ENTINDEX(ent),
            ent->v.origin.x,
            ent->v.origin.y,
            ent->v.origin.z
        );
    }

    void RestoreSavedAmmo(
        void* player,
        const SavedPlayerInventory& inventory
    )
    {
        if (!player || !inventory.valid)
            return;

        for (int i = 0; i < regame::MAX_AMMO_SLOTS; ++i)
        {
            regame::SetAmmo(player, i, inventory.ammo[i]);
        }

        for (int i = 0; i < inventory.weaponCount; ++i)
        {
            const SavedWeapon& saved = inventory.weapons[i];

            if (!saved.valid)
                continue;

            void* item = regame::FindPlayerItem(player, saved.id);

            if (!item)
            {
                continue;
            }

            regame::SetClip(item, saved.clip);
            LH_DEBUG(
                "id=%d item=%p clip=%d",
                saved.id,
                item,
                saved.clip
            );
        }
    }

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

        RestoreSavedAmmo(player, inventory);
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

    LH_DEBUG("Candidate: name=\"%s\" id=%d",
            STRING(ent->v.netname), last_hope_player_id);

    PlayerMethods::RoundRespawn(ent);

    ent->v.deadflag   = DEAD_NO;
    ent->v.health     = 120.0f;
    ent->v.armorvalue = 100.0f;
    ent->v.takedamage = DAMAGE_YES;
    ent->v.solid      = SOLID_SLIDEBOX;
    ent->v.movetype   = MOVETYPE_WALK;
    ent->v.flags     &= ~FL_ONGROUND;

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

    PlayerState& state = g_players[last_hope_player_id];
    RestorePlayerOrigin(ent, state);

    ent->v.iuser1 = 0;
    ent->v.iuser2 = 0;
    ent->v.iuser3 = 0;

    return true;
}

void setCanLastHopeUse(bool last_hope) { can_last_hope_use = last_hope; }
bool getCanLastHopeUse() { return can_last_hope_use; }
