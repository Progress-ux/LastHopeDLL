#pragma once 

class CBasePlayer;

#include "abi/regame_player_abi.h"

constexpr int MAX_PLAYERS = 32;
constexpr int MAX_SAVED_WEAPONS = 32;

struct SavedWeapon
{
    int id = 0;
    int clip = 0;

    int primaryAmmoType = -1;
    int primaryAmmo = 0;

    int secondaryAmmoType = -1;
    int secondaryAmmo = 0;

    bool valid = false;
};

struct SavedPlayerInventory
{
    bool valid = false;
    int activeWeaponId = 0;
    SavedWeapon weapons[MAX_SAVED_WEAPONS]{};
    int weaponCount = 0;
    int ammo[regame::MAX_AMMO_SLOTS]{};
};

struct PlayerState 
{
    bool connected = false;
    bool in_game = false;
    bool initialized = false;
    bool wasAlive = false;

    SavedPlayerInventory inventory{};

    float deathOrigin[3]{};

    void playerConnected(bool alive);
    void playerPutInServer(bool alive);
    void SavePlayerInventory(CBasePlayer* player, int playerId);
};

extern PlayerState g_players[MAX_PLAYERS + 1];
