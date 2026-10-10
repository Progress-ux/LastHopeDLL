#include "weapon.h"

const char* WeaponIdToClassname(int id)
{
    switch (id)
    {
        case 1:  return "weapon_p228";
        case 3:  return "weapon_scout";
        case 4:  return "weapon_hegrenade";
        case 5:  return "weapon_xm1014";
        case 6:  return "weapon_c4";
        case 7:  return "weapon_mac10";
        case 8:  return "weapon_aug";
        case 9:  return "weapon_smokegrenade";
        case 10: return "weapon_elite";
        case 11: return "weapon_fiveseven";
        case 12: return "weapon_ump45";
        case 13: return "weapon_sg550";
        case 14: return "weapon_galil";
        case 15: return "weapon_famas";
        case 16: return "weapon_usp";
        case 17: return "weapon_glock18";
        case 18: return "weapon_awp";
        case 19: return "weapon_mp5navy";
        case 20: return "weapon_m249";
        case 21: return "weapon_m3";
        case 22: return "weapon_m4a1";
        case 23: return "weapon_tmp";
        case 24: return "weapon_g3sg1";
        case 25: return "weapon_flashbang";
        case 26: return "weapon_deagle";
        case 27: return "weapon_sg552";
        case 28: return "weapon_ak47";
        case 29: return "weapon_knife";
        case 30: return "weapon_p90";
        default: return nullptr;
    }
}
