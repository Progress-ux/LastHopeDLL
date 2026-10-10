#pragma once 

bool RegisterCheckWinConditionsHook();
bool RegisterRestartRoundHook();
bool RegisterPlayerKilledHook();

void UnregisterCheckWinConditionsHook();
void UnregisterRestartRoundHook();
void UnregisterPlayerKilledHook();
