#pragma once 

constexpr int MAX_PLAYERS = 32;

struct PlayerState 
{
    bool connected = false;
    bool in_game = false;
    bool initialized = false;
    bool wasAlive = false;

    float deathOrigin[3]{};

    void playerConnected(bool alive);
    void playerPutInServer(bool alive);
};

extern PlayerState g_players[MAX_PLAYERS + 1];
