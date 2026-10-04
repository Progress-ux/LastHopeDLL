#include "player_methods.h"

#include "const.h"
#include "util/logger.h"
#include <cstring>

#define SVC_SETVIEW 5

namespace 
{
    constexpr std::size_t ROUND_RESPAWN_VTABLE_INDEX = 84;

    using RoundRespawnFn = void (*)(void*);
}

namespace PlayerMethods
{
    void RoundRespawn(edict_t *ent)
    {
        if (!ent || !ent->pvPrivateData)
        {
            LH_ERROR(
                "[PlayerMethods] [RoundRespawn] Invalid player/private data"
            );
            return;
        }

        void* player = ent->pvPrivateData;
        void** vtable = *reinterpret_cast<void***>(player);

        auto fn = reinterpret_cast<RoundRespawnFn>(
            vtable[ROUND_RESPAWN_VTABLE_INDEX]
        );
        fn(player);
    }
}
