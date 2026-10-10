#pragma once

#include <cstddef>

namespace regame
{
    // --- CBasePlayer ---
    constexpr std::size_t CBasePlayer_m_rgpPlayerItems = 0x5D0; // CBasePlayerItem* [MAX_ITEM_TYPES]
    constexpr std::size_t CBasePlayer_m_pActiveItem    = 0x5E8;
    constexpr std::size_t CBasePlayer_m_pLastItem      = 0x5F0;
    constexpr std::size_t CBasePlayer_m_rgAmmo         = 0x5F4; // int [MAX_AMMO_SLOTS]
    constexpr std::size_t CBasePlayer_m_rgAmmoLast     = 0x674;

    // --- CBasePlayerItem ---
    constexpr std::size_t CBasePlayerItem_m_pNext = 0xB8;
    constexpr std::size_t CBasePlayerItem_m_iId   = 0xBC;

    // --- CBasePlayerWeapon ---
    constexpr std::size_t CBasePlayerWeapon_m_iPrimaryAmmoType   = 0xD4;
    constexpr std::size_t CBasePlayerWeapon_m_iSecondaryAmmoType = 0xD8;
    constexpr std::size_t CBasePlayerWeapon_m_iClip              = 0xDC;

#ifdef MAX_ITEM_TYPES
#undef MAX_ITEM_TYPES
#endif

#ifdef MAX_AMMO_SLOTS
#undef MAX_AMMO_SLOTS
#endif

constexpr int MAX_ITEM_TYPES = 6;
constexpr int MAX_AMMO_SLOTS = 32;


    inline void* GetActiveItem(void* player)
    {
        if (!player) return nullptr;
        return *reinterpret_cast<void**>(
            static_cast<std::byte*>(player) + CBasePlayer_m_pActiveItem);
    }

    inline void* GetLastItem(void* player)
    {
        if (!player) return nullptr;
        return *reinterpret_cast<void**>(
            static_cast<std::byte*>(player) + CBasePlayer_m_pLastItem);
    }

    inline void* GetSlotItem(void* player, int slot)
    {
        if (!player || slot < 0 || slot >= MAX_ITEM_TYPES) return nullptr;
        auto base = static_cast<std::byte*>(player) + CBasePlayer_m_rgpPlayerItems;
        return *reinterpret_cast<void**>(base + slot * sizeof(void*));
    }

    inline void* GetNextItem(void* item)
    {
        if (!item) return nullptr;
        return *reinterpret_cast<void**>(
            static_cast<std::byte*>(item) + CBasePlayerItem_m_pNext);
    }

    inline int GetItemId(void* item)
    {
        if (!item) return 0;
        return *reinterpret_cast<int*>(
            static_cast<std::byte*>(item) + CBasePlayerItem_m_iId);
    }

    inline void SetAmmo(void* player, int ammoType, int amount)
    {
        if (!player || ammoType < 0 || ammoType >= MAX_AMMO_SLOTS)
            return;

        auto base = static_cast<std::byte*>(player) + CBasePlayer_m_rgAmmo;

        *reinterpret_cast<int*>(
            base + ammoType * sizeof(int)
        ) = amount;
    }

    inline int GetAmmo(void* player, int ammoType)
    {
        if (!player || ammoType < 0 || ammoType >= MAX_AMMO_SLOTS) return 0;
        auto base = static_cast<std::byte*>(player) + CBasePlayer_m_rgAmmo;
        return *reinterpret_cast<int*>(base + ammoType * sizeof(int));
    }

    inline void SetClip(void* weapon, int clip)
    {
        if (!weapon)
            return;

        *reinterpret_cast<int*>(
            static_cast<std::byte*>(weapon) + 
            CBasePlayerWeapon_m_iClip
        ) = clip;
    }

    inline int GetClip(void* weapon)
    {
        if (!weapon) return 0;
        return *reinterpret_cast<int*>(
            static_cast<std::byte*>(weapon) + CBasePlayerWeapon_m_iClip);
    }

    inline int GetPrimaryAmmoType(void* weapon)
    {
        if (!weapon) return 0;
        return *reinterpret_cast<int*>(
            static_cast<std::byte*>(weapon) + CBasePlayerWeapon_m_iPrimaryAmmoType);
    }

    inline int GetSecondaryAmmoType(void* weapon)
    {
        if (!weapon) return 0;
        return *reinterpret_cast<int*>(
            static_cast<std::byte*>(weapon) + CBasePlayerWeapon_m_iSecondaryAmmoType);
    }
    inline void* FindPlayerItem(void* player, int weaponId)
    {
        if (!player || weaponId <= 0)
            return nullptr;
        
        for (int slot = 0; slot < MAX_ITEM_TYPES; ++slot)
        {
            void* item = GetSlotItem(player, slot);

            int traversed = 0;
            while (item && traversed++ < 32)
            {
                if (GetItemId(item) == weaponId)
                    return item;
                item = GetNextItem(item);
            }
        }

        return nullptr;
    }
}
