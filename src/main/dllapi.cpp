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

C_DLLEXPORT int GetEntityAPI2(
    DLL_FUNCTIONS *pFunctionTable, 
    int *interfaceVersion
)
{
	if(!pFunctionTable || !interfaceVersion) 
		return(FALSE);

	if(*interfaceVersion != INTERFACE_VERSION) 
    {
		*interfaceVersion = INTERFACE_VERSION;
		return(FALSE);
	}

    pFunctionTable->pfnClientConnect     = ClientConnect;
    pFunctionTable->pfnClientPutInServer = ClientPutInServer;
    pFunctionTable->pfnClientDisconnect  = ClientDisconnect;
    pFunctionTable->pfnPlayerPostThink   = PlayerPostThink;

	return(TRUE);
}

