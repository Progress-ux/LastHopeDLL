#include "player_state.h"
#include "abi/regame_player_abi.h"
#include "util/logger.h"

PlayerState g_players[MAX_PLAYERS+1];
SavedWeapon g_saved_weapon;

namespace
{
    bool IsValidWeaponId(int id)
    {
        switch (id)
        {
            case 1:  // P228
            case 3:  // Scout
            case 4:  // HE Grenade
            case 5:  // XM1014
            case 6:  // C4
            case 7:  // MAC10
            case 8:  // AUG
            case 9:  // Smoke Grenade
            case 10: // Elite
            case 11: // Five-SeveN
            case 12: // UMP45
            case 13: // SG550
            case 14: // Galil
            case 15: // Famas
            case 16: // USP
            case 17: // Glock18
            case 18: // AWP
            case 19: // MP5
            case 20: // M249
            case 21: // M3
            case 22: // M4A1
            case 23: // TMP
            case 24: // G3SG1
            case 25: // Flashbang
            case 26: // Deagle
            case 27: // SG552
            case 28: // AK47
            case 29: // Knife
            case 30: // P90
                return true;

            default:
                return false;
        }
    }
}

void PlayerState::playerConnected(bool alive)
{
    connected = false;
    wasAlive = alive;
    in_game = true;
    initialized = false;
}

void PlayerState::playerPutInServer(bool alive)
{
    connected = true;
    wasAlive = alive;
    initialized = true;
}

void PlayerState::SavePlayerInventory(CBasePlayer* player, int playerId)
{
    if (!player || playerId < 1 || playerId > MAX_PLAYERS)
    {
        return;
    }

    SavedPlayerInventory& inventory = this->inventory;
    inventory = {};

    void* playerPtr = reinterpret_cast<void*>(player);

    for (int i = 0; i < regame::MAX_AMMO_SLOTS; ++i)
    {
        inventory.ammo[i] = regame::GetAmmo(playerPtr, i);
    }

    void* activeItem = regame::GetActiveItem(playerPtr);

    if (activeItem)
    {
        const int activeId = regame::GetItemId(activeItem);
        if (IsValidWeaponId(activeId))
        {
            inventory.activeWeaponId = activeId;
        }
    }

    constexpr int MAX_SAVED_WEAPONS = 32;

    for (int slot = 0; slot < regame::MAX_ITEM_TYPES; ++slot)
    {
        void* item = regame::GetSlotItem(playerPtr, slot);
        int traversed = 0;

        while (item && traversed < MAX_SAVED_WEAPONS) 
        {
            ++traversed;

            const int weaponId = regame::GetItemId(item);

            if (IsValidWeaponId(weaponId) &&
                inventory.weaponCount < MAX_SAVED_WEAPONS)
            {
                SavedWeapon saved{};

                saved.id = weaponId;
                saved.clip = regame::GetClip(item);

                saved.primaryAmmoType =
                    regame::GetPrimaryAmmoType(item);

                saved.secondaryAmmoType = 
                    regame::GetSecondaryAmmoType(item);

                if (saved.primaryAmmoType >= 0 &&
                    saved.primaryAmmoType < regame::MAX_AMMO_SLOTS)
                {
                    saved.primaryAmmo = 
                        inventory.ammo[saved.primaryAmmoType];
                }

                if (saved.secondaryAmmoType >= 0 &&
                    saved.secondaryAmmoType < regame::MAX_AMMO_SLOTS)
                {
                    saved.secondaryAmmo = 
                        inventory.ammo[saved.secondaryAmmoType];
                }

                saved.valid = true;

                inventory.weapons[inventory.weaponCount++] = saved;
                LH_DEBUG(
                    "player=%d slot=%d weapon=%d "
                    "clip=%d primaryType=%d secondaryType=%d",
                    playerId,
                    slot,
                    saved.id,
                    saved.clip,
                    saved.primaryAmmoType,
                    saved.secondaryAmmoType
                );
            }
            item = regame::GetNextItem(item);
        }

        if (traversed >= MAX_SAVED_WEAPONS)
        {
            LH_DEBUG(
                "[SaveInventory] Traversal limit reached: player=%d slot=%d",
                playerId, slot
            );
        }
    }

    inventory.valid = inventory.weaponCount > 0;
    LH_DEBUG(
        "[SaveInventory] player=%d activeWeapon=%d weapons=%d valid=%d",
        playerId,
        inventory.activeWeaponId,
        inventory.weaponCount,
        inventory.valid
    );
}
