#include "player_methods.h"

#include "util/logger.h"

namespace 
{
    constexpr std::size_t ROUND_RESPAWN_VTABLE_INDEX = 84;

    using RoundRespawnFn = void (*)(void*);
}

namespace PlayerMethods
{
    void RoundRespawn(edict_t *pEntity)
    {
        if (!pEntity || !pEntity->pvPrivateData)
        {
            LH_ERROR("[PlayerMethods] [RoundRespawn] Invalid player/private data");
            return;
        }

        void* player = pEntity->pvPrivateData;
        void** vtable = *reinterpret_cast<void***>(player);

        auto fn = reinterpret_cast<RoundRespawnFn>(
            vtable[ROUND_RESPAWN_VTABLE_INDEX]
        );

        fn(player);
    }
}
