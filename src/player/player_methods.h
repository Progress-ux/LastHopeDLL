#pragma once 

#include <extdll.h>

class CBasePlayer;
class CBaseEntity;

namespace PlayerMethods
{
    void RoundRespawn(edict_t* pEntity);
    void* GiveNamedItem(
        CBasePlayer* player,
        const char* name
    );
}
