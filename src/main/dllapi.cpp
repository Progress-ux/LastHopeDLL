#include <extdll.h>

#include <dllapi.h>
#include <meta_api.h>

#include "const.h"
#include "hooks/entity_hooks.h"
#include "sdk_util.h"

void PlayerPostThink(edict_t *pEntity)
{
    LastHope_PlayerPostThink(pEntity);

    RETURN_META(MRES_IGNORED);
}

static DLL_FUNCTIONS gFunctionTable = 
{
	NULL,					// pfnGameInit
	NULL,					// pfnSpawn
	NULL,					// pfnThink
	NULL,					// pfnUse
	NULL,					// pfnTouch
	NULL,					// pfnBlocked
	NULL,					// pfnKeyValue
	NULL,					// pfnSave
	NULL,					// pfnRestore
	NULL,					// pfnSetAbsBox

	NULL,					// pfnSaveWriteFields
	NULL,					// pfnSaveReadFields

	NULL,					// pfnSaveGlobalState
	NULL,					// pfnRestoreGlobalState
	NULL,					// pfnResetGlobalState

	NULL,					// pfnClientConnect
	NULL,					// pfnClientDisconnect
	NULL,					// pfnClientKill
	NULL,					// pfnClientPutInServer
	NULL,					// pfnClientCommand
	NULL,					// pfnClientUserInfoChanged
	NULL,					// pfnServerActivate
	NULL,					// pfnServerDeactivate

	NULL,					// pfnPlayerPreThink
	PlayerPostThink,					// pfnPlayerPostThink

	NULL,					// pfnStartFrame
	NULL,					// pfnParmsNewLevel
	NULL,					// pfnParmsChangeLevel

	NULL,					// pfnGetGameDescription
	NULL,					// pfnPlayerCustomization

	NULL,					// pfnSpectatorConnect
	NULL,					// pfnSpectatorDisconnect
	NULL,					// pfnSpectatorThink
	
	NULL,					// pfnSys_Error

	NULL,					// pfnPM_Move
	NULL,					// pfnPM_Init
	NULL,					// pfnPM_FindTextureType
	
	NULL,					// pfnSetupVisibility
	NULL,					// pfnUpdateClientData
	NULL,					// pfnAddToFullPack
	NULL,					// pfnCreateBaseline
	NULL,					// pfnRegisterEncoders
	NULL,					// pfnGetWeaponData
	NULL,					// pfnCmdStart
	NULL,					// pfnCmdEnd
	NULL,					// pfnConnectionlessPacket
	NULL,					// pfnGetHullBounds
	NULL,					// pfnCreateInstancedBaselines
	NULL,					// pfnInconsistentFile
	NULL,					// pfnAllowLagCompensation
};

qboolean ClientConnect(
    edict_t* pEntity,
    const char* pszName,
    const char* pszAddress,
    char szRejectReason[128]
)
{
    qboolean result = LastHope_ClientConnect(
        pEntity,
        pszName,
        pszAddress,
        szRejectReason
    );

    RETURN_META_VALUE(MRES_IGNORED, result);
}

void ClientPutInServer(edict_t *pEntity)
{
    LastHope_ClientPutInServer(pEntity);
    RETURN_META(MRES_IGNORED);
}

void ClientDisconnect(edict_t *pEntity)
{
    LastHope_ClientDisconnect(pEntity);
    RETURN_META(MRES_IGNORED);
}

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, 
		int *interfaceVersion)
{
	if(!pFunctionTable) {
		UTIL_LogPrintf("GetEntityAPI2 called with null pFunctionTable");
		return(FALSE);
	}
	else if(*interfaceVersion != INTERFACE_VERSION) {
		UTIL_LogPrintf("GetEntityAPI2 version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
		//! Tell metamod what version we had, so it can figure out who is out of date.
		*interfaceVersion = INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));

    pFunctionTable->pfnClientConnect     =  ClientConnect;
    pFunctionTable->pfnClientPutInServer =  ClientPutInServer;
    pFunctionTable->pfnClientDisconnect  =  ClientDisconnect;

	return(TRUE);
}

