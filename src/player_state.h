#pragma once 

constexpr int MAX_PLAYERS = 32;

struct PlayerState 
{
    bool connected = false;
    bool in_game = false;
    bool initialized = false;
    bool wasAlive = false;
    bool last_hope = false;

    float deathOrigin[3]{};

    int savedWeapon[32]{};
    int savedWeaponCount = 0;

    void playerConnected(bool alive);
    void playerPutInServer(bool alive);
};

extern PlayerState g_players[MAX_PLAYERS + 1];
