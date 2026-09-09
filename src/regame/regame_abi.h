#pragma once 

#include <cstddef>

namespace regame 
{
    constexpr const char* API_VERSION = 
        "VRE_GAMEDLL_API_VERSION001";

    class ReGameFuncs_t;
    class IReGameHookchains;
    class IReGameFuncs;
    class CGameRules;

    struct WeaponInfoStruct;
    struct playermove_s;
    struct WeaponSlotInfo;
    struct ItemInfo;
    struct AmmoInfo;
    struct AmmoInfoStruct;

    enum WeaponIdType : int;
    enum AmmoType : int;

    class IReGameApi {
    public:
        virtual ~IReGameApi() {}

        virtual int GetMajorVersion() = 0;
        virtual int GetMinorVersion() = 0;
        virtual const ReGameFuncs_t *GetFuncs() = 0;
        virtual IReGameHookchains *GetHookchains() = 0;

        virtual class CGameRules *GetGameRules() = 0;
        virtual struct WeaponInfoStruct *GetWeaponInfo(int weaponID) = 0;
        virtual struct WeaponInfoStruct *GetWeaponInfo(const char *weaponName) = 0;
        virtual struct playermove_s *GetPlayerMove() = 0;
        virtual struct WeaponSlotInfo *GetWeaponSlot(WeaponIdType weaponID) = 0;
        virtual struct WeaponSlotInfo *GetWeaponSlot(const char *weaponName) = 0;
        virtual struct ItemInfo *GetItemInfo(WeaponIdType weaponID) = 0;
        virtual struct AmmoInfo *GetAmmoInfo(AmmoType ammoID) = 0;
        virtual struct AmmoInfoStruct *GetAmmoInfoEx(AmmoType ammoID) = 0;
        virtual struct AmmoInfoStruct *GetAmmoInfoEx(const char *ammoName) = 0;
        virtual bool BGetICSEntity(const char *pchVersion) const = 0;
        virtual bool BGetIGameRules(const char *pchVersion) const = 0;
    };

    using CreateInterfaceFn = 
        void *(*)(const char *name, int* returnCode);
    
} // namespace regame
