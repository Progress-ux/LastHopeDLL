#include "player_state.h"

PlayerState g_players[MAX_PLAYERS+1];

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
