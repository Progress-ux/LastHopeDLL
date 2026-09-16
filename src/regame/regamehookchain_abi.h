#pragma once

namespace regame
{

	enum HookChainPriority
	{
		HC_PRIORITY_UNINTERRUPTABLE = 255,  // Hook will be called before other hooks.
		HC_PRIORITY_HIGH = 192,             // Hook will be called before hooks with default priority.
		HC_PRIORITY_DEFAULT = 128,          // Default hook call priority.
		HC_PRIORITY_MEDIUM = 64,            // Hook will be called after hooks with default priority.
		HC_PRIORITY_LOW = 0,                // Hook will be called after all other hooks.
	};

	class IReGameHookRegistry_CBasePlayer_Spawn;
	class IReGameHookRegistry_CBasePlayer_Precache;
	class IReGameHookRegistry_CBasePlayer_ObjectCaps;
	class IReGameHookRegistry_CBasePlayer_Classify;
	class IReGameHookRegistry_CBasePlayer_TraceAttack;
	class IReGameHookRegistry_CBasePlayer_TakeDamage;
	class IReGameHookRegistry_CBasePlayer_TakeHealth;
	class IReGameHookRegistry_CBasePlayer_Killed;
	class IReGameHookRegistry_CBasePlayer_AddPoints;
	class IReGameHookRegistry_CBasePlayer_AddPointsToTeam;
	class IReGameHookRegistry_CBasePlayer_AddPlayerItem;
	class IReGameHookRegistry_CBasePlayer_RemovePlayerItem;
	class IReGameHookRegistry_CBasePlayer_GiveAmmo;
	class IReGameHookRegistry_CBasePlayer_ResetMaxSpeed;
	class IReGameHookRegistry_CBasePlayer_Jump;
	class IReGameHookRegistry_CBasePlayer_Duck;
	class IReGameHookRegistry_CBasePlayer_PreThink;
	class IReGameHookRegistry_CBasePlayer_PostThink;
	class IReGameHookRegistry_CBasePlayer_UpdateClientData;
	class IReGameHookRegistry_CBasePlayer_ImpulseCommands;
	class IReGameHookRegistry_CBasePlayer_RoundRespawn;
	class IReGameHookRegistry_CBasePlayer_Blind;
	class IReGameHookRegistry_CBasePlayer_Observer_IsValidTarget;
	class IReGameHookRegistry_CBasePlayer_SetAnimation;
	class IReGameHookRegistry_CBasePlayer_GiveDefaultItems;
	class IReGameHookRegistry_CBasePlayer_GiveNamedItem;
	class IReGameHookRegistry_CBasePlayer_AddAccount;
	class IReGameHookRegistry_CBasePlayer_GiveShield;
	class IReGameHookRegistry_CBasePlayer_SetClientUserInfoModel;
	class IReGameHookRegistry_CBasePlayer_SetClientUserInfoName;
	class IReGameHookRegistry_CBasePlayer_HasRestrictItem;
	class IReGameHookRegistry_CBasePlayer_DropPlayerItem;
	class IReGameHookRegistry_CBasePlayer_DropShield;
	class IReGameHookRegistry_CBasePlayer_OnSpawnEquip;
	class IReGameHookRegistry_CBasePlayer_Radio;
	class IReGameHookRegistry_CBasePlayer_Disappear;
	class IReGameHookRegistry_CBasePlayer_MakeVIP;
	class IReGameHookRegistry_CBasePlayer_MakeBomber;
	class IReGameHookRegistry_CBasePlayer_StartObserver;
	class IReGameHookRegistry_CBasePlayer_GetIntoGame;
	class IReGameHookRegistry_CBaseAnimating_ResetSequenceInfo;
	class IReGameHookRegistry_GetForceCamera;
	class IReGameHookRegistry_PlayerBlind;
	class IReGameHookRegistry_RadiusFlash_TraceLine;
	class IReGameHookRegistry_RoundEnd;
	class IReGameHookRegistry_InstallGameRules;
	class IReGameHookRegistry_PM_Init;
	class IReGameHookRegistry_PM_Move;
	class IReGameHookRegistry_PM_AirMove;
	class IReGameHookRegistry_HandleMenu_ChooseAppearance;
	class IReGameHookRegistry_HandleMenu_ChooseTeam;
	class IReGameHookRegistry_ShowMenu;
	class IReGameHookRegistry_ShowVGUIMenu;
	class IReGameHookRegistry_BuyGunAmmo;
	class IReGameHookRegistry_BuyWeaponByWeaponID;
	class IReGameHookRegistry_InternalCommand;
	class IReGameHookRegistry_CSGameRules_FShouldSwitchWeapon;
	class IReGameHookRegistry_CSGameRules_GetNextBestWeapon;
	class IReGameHookRegistry_CSGameRules_FlPlayerFallDamage;
	class IReGameHookRegistry_CSGameRules_FPlayerCanTakeDamage;
	class IReGameHookRegistry_CSGameRules_PlayerSpawn;
	class IReGameHookRegistry_CSGameRules_FPlayerCanRespawn;
	class IReGameHookRegistry_CSGameRules_GetPlayerSpawnSpot;
	class IReGameHookRegistry_CSGameRules_ClientUserInfoChanged;
	class IReGameHookRegistry_CSGameRules_PlayerKilled;
	class IReGameHookRegistry_CSGameRules_DeathNotice;
	class IReGameHookRegistry_CSGameRules_CanHavePlayerItem;
	class IReGameHookRegistry_CSGameRules_DeadPlayerWeapons;
	class IReGameHookRegistry_CSGameRules_ServerDeactivate;
	class IReGameHookRegistry_CSGameRules_CheckMapConditions;
	class IReGameHookRegistry_CSGameRules_CleanUpMap;
	// class IReGameHookRegistry_CSGameRules_RestartRound;

	// class IReGameHookRegistry_CSGameRules_CheckWinConditions;

	class IReGameHookRegistry_CSGameRules_RemoveGuns;
	class IReGameHookRegistry_CSGameRules_GiveC4;
	class IReGameHookRegistry_CSGameRules_ChangeLevel;
	class IReGameHookRegistry_CSGameRules_GoToIntermission;
	class IReGameHookRegistry_CSGameRules_BalanceTeams;
	class IReGameHookRegistry_CSGameRules_OnRoundFreezeEnd;
	class IReGameHookRegistry_PM_UpdateStepSound;
	class IReGameHookRegistry_CBasePlayer_StartDeathCam;
	class IReGameHookRegistry_CBasePlayer_SwitchTeam;
	class IReGameHookRegistry_CBasePlayer_CanSwitchTeam;
	class IReGameHookRegistry_CBasePlayer_ThrowGrenade;
	class IReGameHookRegistry_CSGameRules_CanPlayerHearPlayer;
	class IReGameHookRegistry_CWeaponBox_SetModel;
	class IReGameHookRegistry_CGrenade_DefuseBombStart;
	class IReGameHookRegistry_CGrenade_DefuseBombEnd;
	class IReGameHookRegistry_CGrenade_ExplodeHeGrenade;
	class IReGameHookRegistry_CGrenade_ExplodeFlashbang;
	class IReGameHookRegistry_CGrenade_ExplodeSmokeGrenade;
	class IReGameHookRegistry_CGrenade_ExplodeBomb;
	class IReGameHookRegistry_ThrowHeGrenade;
	class IReGameHookRegistry_ThrowFlashbang;
	class IReGameHookRegistry_ThrowSmokeGrenade;
	class IReGameHookRegistry_PlantBomb;
	class IReGameHookRegistry_CBasePlayer_RemoveSpawnProtection;
	class IReGameHookRegistry_CBasePlayer_SetSpawnProtection;
	class IReGameHookRegistry_IsPenetrableEntity;
	class IReGameHookRegistry_CBasePlayer_HintMessageEx;
	class IReGameHookRegistry_CBasePlayer_UseEmpty;
	class IReGameHookRegistry_CBasePlayerWeapon_CanDeploy;
	class IReGameHookRegistry_CBasePlayerWeapon_DefaultDeploy;
	class IReGameHookRegistry_CBasePlayerWeapon_DefaultReload;
	class IReGameHookRegistry_CBasePlayerWeapon_DefaultShotgunReload;
	class IReGameHookRegistry_CBasePlayer_DropIdlePlayer;
	class IReGameHookRegistry_CreateWeaponBox;
	class IReGameHookRegistry_SpawnHeadGib;
	class IReGameHookRegistry_SpawnRandomGibs;
	class IReGameHookRegistry_CGib_Spawn;
	class IReGameHookRegistry_CGib_BounceGibTouch;
	class IReGameHookRegistry_CGib_WaitTillLand;
	class IReGameHookRegistry_CBaseEntity_FireBullets;
	class IReGameHookRegistry_CBaseEntity_FireBuckshots;
	class IReGameHookRegistry_CBaseEntity_FireBullets3;
	class IReGameHookRegistry_CBasePlayer_Observer_SetMode;
	class IReGameHookRegistry_CBasePlayer_Observer_FindNextPlayer;
	class IReGameHookRegistry_CBasePlayer_Pain;
	class IReGameHookRegistry_CBasePlayer_DeathSound;
	class IReGameHookRegistry_CBasePlayer_JoiningThink;
	class IReGameHookRegistry_FreeGameRules;
	class IReGameHookRegistry_PM_LadderMove;
	class IReGameHookRegistry_PM_WaterJump;
	class IReGameHookRegistry_PM_CheckWaterJump;
	class IReGameHookRegistry_PM_Jump;
	class IReGameHookRegistry_PM_Duck;
	class IReGameHookRegistry_PM_UnDuck;
	class IReGameHookRegistry_PM_PlayStepSound;
	class IReGameHookRegistry_PM_AirAccelerate;
	class IReGameHookRegistry_ClearMultiDamage;
	class IReGameHookRegistry_AddMultiDamage;
	class IReGameHookRegistry_ApplyMultiDamage;
	class IReGameHookRegistry_BuyItem;
	class IReGameHookRegistry_CSGameRules_Think;
	class IReGameHookRegistry_CSGameRules_TeamFull;
	class IReGameHookRegistry_CSGameRules_TeamStacked;
	class IReGameHookRegistry_CSGameRules_PlayerGotWeapon;
	class IReGameHookRegistry_CBotManager_OnEvent;
	class IReGameHookRegistry_CBasePlayer_CheckTimeBasedDamage;
	class IReGameHookRegistry_CBasePlayer_EntSelectSpawnPoint;
	class IReGameHookRegistry_CBasePlayerWeapon_ItemPostFrame;
	class IReGameHookRegistry_CBasePlayerWeapon_KickBack;
	class IReGameHookRegistry_CBasePlayerWeapon_SendWeaponAnim;
	class IReGameHookRegistry_CSGameRules_SendDeathMessage;
	class IReGameHookRegistry_CBasePlayer_PlayerDeathThink;
	class IReGameHookRegistry_CBasePlayer_Observer_Think;
	class IReGameHookRegistry_CBasePlayer_RemoveAllItems;
	class IReGameHookRegistry_CBasePlayer_UpdateStatusBar;
	class IReGameHookRegistry_CBasePlayer_TakeDamageImpulse;
	class IReGameHookRegistry_SendSayMessage;

	template<typename t_ret, typename ...t_args>
	class IHookChain
	{
	protected:
		virtual ~IHookChain() {}
	public:
		virtual t_ret callNext(t_args... args) = 0;
		virtual t_ret callOriginal(t_args... args) = 0;
	};

    template<typename t_ret, typename ...t_args>
    class IHookChainRegistry
    {
    public:
        typedef t_ret(*hookfunc_t)(
            IHookChain<t_ret, t_args...>*,
            t_args...
        );

        virtual void registerHook(
            hookfunc_t hook,
            int priority = HC_PRIORITY_DEFAULT
        ) = 0;

        virtual void unregisterHook(
            hookfunc_t hook
        ) = 0;
    };

    using IReGameHookRegistry_CSGameRules_CheckWinConditions = 
        IHookChainRegistry<void>;
	using CheckWinConditionsRegistry = IHookChainRegistry<void>;

    using IReGameHookRegistry_CSGameRules_RestartRound = 
        IHookChainRegistry<void>;

	class IReGameHookchains
	{
	public:
		virtual ~IReGameHookchains() {}

		// CBasePlayer virtual
		virtual IReGameHookRegistry_CBasePlayer_Spawn *CBasePlayer_Spawn() = 0;
		virtual IReGameHookRegistry_CBasePlayer_Precache *CBasePlayer_Precache() = 0;
		virtual IReGameHookRegistry_CBasePlayer_ObjectCaps *CBasePlayer_ObjectCaps() = 0;
		virtual IReGameHookRegistry_CBasePlayer_Classify *CBasePlayer_Classify() = 0;
		virtual IReGameHookRegistry_CBasePlayer_TraceAttack *CBasePlayer_TraceAttack() = 0;
		virtual IReGameHookRegistry_CBasePlayer_TakeDamage *CBasePlayer_TakeDamage() = 0;
		virtual IReGameHookRegistry_CBasePlayer_TakeHealth *CBasePlayer_TakeHealth() = 0;
		virtual IReGameHookRegistry_CBasePlayer_Killed *CBasePlayer_Killed() = 0;
		virtual IReGameHookRegistry_CBasePlayer_AddPoints *CBasePlayer_AddPoints() = 0;
		virtual IReGameHookRegistry_CBasePlayer_AddPointsToTeam *CBasePlayer_AddPointsToTeam() = 0;
		virtual IReGameHookRegistry_CBasePlayer_AddPlayerItem *CBasePlayer_AddPlayerItem() = 0;
		virtual IReGameHookRegistry_CBasePlayer_RemovePlayerItem *CBasePlayer_RemovePlayerItem() = 0;
		virtual IReGameHookRegistry_CBasePlayer_GiveAmmo *CBasePlayer_GiveAmmo() = 0;
		virtual IReGameHookRegistry_CBasePlayer_ResetMaxSpeed *CBasePlayer_ResetMaxSpeed() = 0;
		virtual IReGameHookRegistry_CBasePlayer_Jump *CBasePlayer_Jump() = 0;
		virtual IReGameHookRegistry_CBasePlayer_Duck *CBasePlayer_Duck() = 0;
		virtual IReGameHookRegistry_CBasePlayer_PreThink *CBasePlayer_PreThink() = 0;
		virtual IReGameHookRegistry_CBasePlayer_PostThink *CBasePlayer_PostThink() = 0;
		virtual IReGameHookRegistry_CBasePlayer_UpdateClientData *CBasePlayer_UpdateClientData() = 0;
		virtual IReGameHookRegistry_CBasePlayer_ImpulseCommands *CBasePlayer_ImpulseCommands() = 0;
		virtual IReGameHookRegistry_CBasePlayer_RoundRespawn *CBasePlayer_RoundRespawn() = 0;
		virtual IReGameHookRegistry_CBasePlayer_Blind *CBasePlayer_Blind() = 0;

		virtual IReGameHookRegistry_CBasePlayer_Observer_IsValidTarget *CBasePlayer_Observer_IsValidTarget() = 0;
		virtual IReGameHookRegistry_CBasePlayer_SetAnimation *CBasePlayer_SetAnimation() = 0;
		virtual IReGameHookRegistry_CBasePlayer_GiveDefaultItems *CBasePlayer_GiveDefaultItems() = 0;
		virtual IReGameHookRegistry_CBasePlayer_GiveNamedItem *CBasePlayer_GiveNamedItem() = 0;
		virtual IReGameHookRegistry_CBasePlayer_AddAccount *CBasePlayer_AddAccount() = 0;
		virtual IReGameHookRegistry_CBasePlayer_GiveShield *CBasePlayer_GiveShield() = 0;
		virtual IReGameHookRegistry_CBasePlayer_SetClientUserInfoModel *CBasePlayer_SetClientUserInfoModel() = 0;
		virtual IReGameHookRegistry_CBasePlayer_SetClientUserInfoName *CBasePlayer_SetClientUserInfoName() = 0;
		virtual IReGameHookRegistry_CBasePlayer_HasRestrictItem *CBasePlayer_HasRestrictItem() = 0;
		virtual IReGameHookRegistry_CBasePlayer_DropPlayerItem *CBasePlayer_DropPlayerItem() = 0;
		virtual IReGameHookRegistry_CBasePlayer_DropShield *CBasePlayer_DropShield() = 0;
		virtual IReGameHookRegistry_CBasePlayer_OnSpawnEquip *CBasePlayer_OnSpawnEquip() = 0;
		virtual IReGameHookRegistry_CBasePlayer_Radio *CBasePlayer_Radio() = 0;
		virtual IReGameHookRegistry_CBasePlayer_Disappear *CBasePlayer_Disappear() = 0;
		virtual IReGameHookRegistry_CBasePlayer_MakeVIP *CBasePlayer_MakeVIP() = 0;
		virtual IReGameHookRegistry_CBasePlayer_MakeBomber *CBasePlayer_MakeBomber() = 0;
		virtual IReGameHookRegistry_CBasePlayer_StartObserver *CBasePlayer_StartObserver() = 0;
		virtual IReGameHookRegistry_CBasePlayer_GetIntoGame *CBasePlayer_GetIntoGame() = 0;

		virtual IReGameHookRegistry_CBaseAnimating_ResetSequenceInfo *CBaseAnimating_ResetSequenceInfo() = 0;

		virtual IReGameHookRegistry_GetForceCamera *GetForceCamera() = 0;
		virtual IReGameHookRegistry_PlayerBlind *PlayerBlind() = 0;
		virtual IReGameHookRegistry_RadiusFlash_TraceLine *RadiusFlash_TraceLine() = 0;
		virtual IReGameHookRegistry_RoundEnd *RoundEnd() = 0;
		virtual IReGameHookRegistry_InstallGameRules *InstallGameRules() = 0;
		virtual IReGameHookRegistry_PM_Init *PM_Init() = 0;
		virtual IReGameHookRegistry_PM_Move *PM_Move() = 0;
		virtual IReGameHookRegistry_PM_AirMove *PM_AirMove() = 0;
		virtual IReGameHookRegistry_HandleMenu_ChooseAppearance *HandleMenu_ChooseAppearance() = 0;
		virtual IReGameHookRegistry_HandleMenu_ChooseTeam *HandleMenu_ChooseTeam() = 0;
		virtual IReGameHookRegistry_ShowMenu *ShowMenu() = 0;
		virtual IReGameHookRegistry_ShowVGUIMenu *ShowVGUIMenu() = 0;
		virtual IReGameHookRegistry_BuyGunAmmo *BuyGunAmmo() = 0;
		virtual IReGameHookRegistry_BuyWeaponByWeaponID *BuyWeaponByWeaponID() = 0;
		virtual IReGameHookRegistry_InternalCommand *InternalCommand() = 0;

		virtual IReGameHookRegistry_CSGameRules_FShouldSwitchWeapon *CSGameRules_FShouldSwitchWeapon() = 0;
		virtual IReGameHookRegistry_CSGameRules_GetNextBestWeapon *CSGameRules_GetNextBestWeapon() = 0;
		virtual IReGameHookRegistry_CSGameRules_FlPlayerFallDamage *CSGameRules_FlPlayerFallDamage() = 0;
		virtual IReGameHookRegistry_CSGameRules_FPlayerCanTakeDamage *CSGameRules_FPlayerCanTakeDamage() = 0;
		virtual IReGameHookRegistry_CSGameRules_PlayerSpawn *CSGameRules_PlayerSpawn() = 0;
		virtual IReGameHookRegistry_CSGameRules_FPlayerCanRespawn *CSGameRules_FPlayerCanRespawn() = 0;
		virtual IReGameHookRegistry_CSGameRules_GetPlayerSpawnSpot *CSGameRules_GetPlayerSpawnSpot() = 0;
		virtual IReGameHookRegistry_CSGameRules_ClientUserInfoChanged *CSGameRules_ClientUserInfoChanged() = 0;
		virtual IReGameHookRegistry_CSGameRules_PlayerKilled *CSGameRules_PlayerKilled() = 0;
		virtual IReGameHookRegistry_CSGameRules_DeathNotice *CSGameRules_DeathNotice() = 0;
		virtual IReGameHookRegistry_CSGameRules_CanHavePlayerItem *CSGameRules_CanHavePlayerItem() = 0;
		virtual IReGameHookRegistry_CSGameRules_DeadPlayerWeapons *CSGameRules_DeadPlayerWeapons() = 0;
		virtual IReGameHookRegistry_CSGameRules_ServerDeactivate *CSGameRules_ServerDeactivate() = 0;
		virtual IReGameHookRegistry_CSGameRules_CheckMapConditions *CSGameRules_CheckMapConditions() = 0;
		virtual IReGameHookRegistry_CSGameRules_CleanUpMap *CSGameRules_CleanUpMap() = 0;
		virtual IReGameHookRegistry_CSGameRules_RestartRound *CSGameRules_RestartRound() = 0;
		virtual IReGameHookRegistry_CSGameRules_CheckWinConditions *CSGameRules_CheckWinConditions() = 0;
		virtual IReGameHookRegistry_CSGameRules_RemoveGuns *CSGameRules_RemoveGuns() = 0;
		virtual IReGameHookRegistry_CSGameRules_GiveC4 *CSGameRules_GiveC4() = 0;
		virtual IReGameHookRegistry_CSGameRules_ChangeLevel *CSGameRules_ChangeLevel() = 0;
		virtual IReGameHookRegistry_CSGameRules_GoToIntermission *CSGameRules_GoToIntermission() = 0;
		virtual IReGameHookRegistry_CSGameRules_BalanceTeams *CSGameRules_BalanceTeams() = 0;
		virtual IReGameHookRegistry_CSGameRules_OnRoundFreezeEnd *CSGameRules_OnRoundFreezeEnd() = 0;
		virtual IReGameHookRegistry_PM_UpdateStepSound *PM_UpdateStepSound() = 0;
		virtual IReGameHookRegistry_CBasePlayer_StartDeathCam *CBasePlayer_StartDeathCam() = 0;
		virtual IReGameHookRegistry_CBasePlayer_SwitchTeam *CBasePlayer_SwitchTeam() = 0;
		virtual IReGameHookRegistry_CBasePlayer_CanSwitchTeam *CBasePlayer_CanSwitchTeam() = 0;
		virtual IReGameHookRegistry_CBasePlayer_ThrowGrenade *CBasePlayer_ThrowGrenade() = 0;
		virtual IReGameHookRegistry_CSGameRules_CanPlayerHearPlayer *CSGameRules_CanPlayerHearPlayer() = 0;
		virtual IReGameHookRegistry_CWeaponBox_SetModel *CWeaponBox_SetModel() = 0;
		virtual IReGameHookRegistry_CGrenade_DefuseBombStart *CGrenade_DefuseBombStart() = 0;
		virtual IReGameHookRegistry_CGrenade_DefuseBombEnd *CGrenade_DefuseBombEnd() = 0;
		virtual IReGameHookRegistry_CGrenade_ExplodeHeGrenade *CGrenade_ExplodeHeGrenade() = 0;
		virtual IReGameHookRegistry_CGrenade_ExplodeFlashbang *CGrenade_ExplodeFlashbang() = 0;
		virtual IReGameHookRegistry_CGrenade_ExplodeSmokeGrenade *CGrenade_ExplodeSmokeGrenade() = 0;
		virtual IReGameHookRegistry_CGrenade_ExplodeBomb *CGrenade_ExplodeBomb() = 0;
		virtual IReGameHookRegistry_ThrowHeGrenade *ThrowHeGrenade() = 0;
		virtual IReGameHookRegistry_ThrowFlashbang *ThrowFlashbang() = 0;
		virtual IReGameHookRegistry_ThrowSmokeGrenade *ThrowSmokeGrenade() = 0;
		virtual IReGameHookRegistry_PlantBomb *PlantBomb() = 0;
		virtual IReGameHookRegistry_CBasePlayer_RemoveSpawnProtection *CBasePlayer_RemoveSpawnProtection() = 0;
		virtual IReGameHookRegistry_CBasePlayer_SetSpawnProtection *CBasePlayer_SetSpawnProtection() = 0;
		virtual IReGameHookRegistry_IsPenetrableEntity *IsPenetrableEntity() = 0;
		virtual IReGameHookRegistry_CBasePlayer_HintMessageEx *CBasePlayer_HintMessageEx() = 0;
		virtual IReGameHookRegistry_CBasePlayer_UseEmpty *CBasePlayer_UseEmpty() = 0;
		virtual IReGameHookRegistry_CBasePlayerWeapon_CanDeploy *CBasePlayerWeapon_CanDeploy() = 0;
		virtual IReGameHookRegistry_CBasePlayerWeapon_DefaultDeploy *CBasePlayerWeapon_DefaultDeploy() = 0;
		virtual IReGameHookRegistry_CBasePlayerWeapon_DefaultReload *CBasePlayerWeapon_DefaultReload() = 0;
		virtual IReGameHookRegistry_CBasePlayerWeapon_DefaultShotgunReload *CBasePlayerWeapon_DefaultShotgunReload() = 0;
		virtual IReGameHookRegistry_CBasePlayer_DropIdlePlayer *CBasePlayer_DropIdlePlayer() = 0;
		virtual IReGameHookRegistry_CreateWeaponBox *CreateWeaponBox() = 0;

		virtual IReGameHookRegistry_SpawnHeadGib *SpawnHeadGib() = 0;
		virtual IReGameHookRegistry_SpawnRandomGibs *SpawnRandomGibs() = 0;
		virtual IReGameHookRegistry_CGib_Spawn *CGib_Spawn() = 0;
		virtual IReGameHookRegistry_CGib_BounceGibTouch *CGib_BounceGibTouch() = 0;
		virtual IReGameHookRegistry_CGib_WaitTillLand *CGib_WaitTillLand() = 0;

		virtual IReGameHookRegistry_CBaseEntity_FireBullets *CBaseEntity_FireBullets() = 0;
		virtual IReGameHookRegistry_CBaseEntity_FireBuckshots *CBaseEntity_FireBuckshots() = 0;
		virtual IReGameHookRegistry_CBaseEntity_FireBullets3 *CBaseEntity_FireBullets3() = 0;

		virtual IReGameHookRegistry_CBasePlayer_Observer_SetMode *CBasePlayer_Observer_SetMode() = 0;
		virtual IReGameHookRegistry_CBasePlayer_Observer_FindNextPlayer *CBasePlayer_Observer_FindNextPlayer() = 0;

		virtual IReGameHookRegistry_CBasePlayer_Pain *CBasePlayer_Pain() = 0;
		virtual IReGameHookRegistry_CBasePlayer_DeathSound *CBasePlayer_DeathSound() = 0;
		virtual IReGameHookRegistry_CBasePlayer_JoiningThink *CBasePlayer_JoiningThink() = 0;

		virtual IReGameHookRegistry_FreeGameRules *FreeGameRules() = 0;
		virtual IReGameHookRegistry_PM_LadderMove *PM_LadderMove() = 0;
		virtual IReGameHookRegistry_PM_WaterJump *PM_WaterJump() = 0;
		virtual IReGameHookRegistry_PM_CheckWaterJump *PM_CheckWaterJump() = 0;
		virtual IReGameHookRegistry_PM_Jump *PM_Jump() = 0;
		virtual IReGameHookRegistry_PM_Duck *PM_Duck() = 0;
		virtual IReGameHookRegistry_PM_UnDuck *PM_UnDuck() = 0;
		virtual IReGameHookRegistry_PM_PlayStepSound *PM_PlayStepSound() = 0;
		virtual IReGameHookRegistry_PM_AirAccelerate *PM_AirAccelerate() = 0;
		virtual IReGameHookRegistry_ClearMultiDamage *ClearMultiDamage() = 0;
		virtual IReGameHookRegistry_AddMultiDamage *AddMultiDamage() = 0;
		virtual IReGameHookRegistry_ApplyMultiDamage *ApplyMultiDamage() = 0;
		virtual IReGameHookRegistry_BuyItem *BuyItem() = 0;
		virtual IReGameHookRegistry_CSGameRules_Think *CSGameRules_Think() = 0;
		virtual IReGameHookRegistry_CSGameRules_TeamFull *CSGameRules_TeamFull() = 0;
		virtual IReGameHookRegistry_CSGameRules_TeamStacked *CSGameRules_TeamStacked() = 0;
		virtual IReGameHookRegistry_CSGameRules_PlayerGotWeapon *CSGameRules_PlayerGotWeapon() = 0;
		virtual IReGameHookRegistry_CBotManager_OnEvent *CBotManager_OnEvent() = 0;
		virtual IReGameHookRegistry_CBasePlayer_CheckTimeBasedDamage *CBasePlayer_CheckTimeBasedDamage() = 0;
		virtual IReGameHookRegistry_CBasePlayer_EntSelectSpawnPoint *CBasePlayer_EntSelectSpawnPoint() = 0;
		virtual IReGameHookRegistry_CBasePlayerWeapon_ItemPostFrame *CBasePlayerWeapon_ItemPostFrame() = 0;
		virtual IReGameHookRegistry_CBasePlayerWeapon_KickBack *CBasePlayerWeapon_KickBack() = 0;
		virtual IReGameHookRegistry_CBasePlayerWeapon_SendWeaponAnim *CBasePlayerWeapon_SendWeaponAnim() = 0;
		virtual IReGameHookRegistry_CSGameRules_SendDeathMessage *CSGameRules_SendDeathMessage() = 0;

		virtual IReGameHookRegistry_CBasePlayer_PlayerDeathThink *CBasePlayer_PlayerDeathThink() = 0;
		virtual IReGameHookRegistry_CBasePlayer_Observer_Think *CBasePlayer_Observer_Think() = 0;
		virtual IReGameHookRegistry_CBasePlayer_RemoveAllItems *CBasePlayer_RemoveAllItems() = 0;

		virtual IReGameHookRegistry_CBasePlayer_UpdateStatusBar *CBasePlayer_UpdateStatusBar() = 0;
		virtual IReGameHookRegistry_CBasePlayer_TakeDamageImpulse *CBasePlayer_TakeDamageImpulse() = 0;

		virtual IReGameHookRegistry_SendSayMessage *SendSayMessage() = 0;
	};
}
