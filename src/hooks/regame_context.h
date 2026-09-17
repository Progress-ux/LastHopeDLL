#pragma once 

#include "abi/regame_abi.h"
#include "abi/regamehookchain_abi.h"

namespace ReGameContext 
{
    bool Init(regame::IReGameApi* api);
    
    void Reset();

    bool isInitialize();

    regame::IReGameApi*         Api();
    regame::IReGameHookchains*  Hookchains();
}
