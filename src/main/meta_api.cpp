#include <extdll.h>			// always
#include <meta_api.h>		// of course

#include "util/logger.h"
#include "mutil.h"
#include "osdep.h"
#include "hooks/regame_loader.h"

// Must provide at least one of these..
static META_FUNCTIONS gMetaFunctionTable = {
	NULL,			// pfnGetEntityAPI				HL SDK; called before game DLL
	NULL,			// pfnGetEntityAPI_Post			META; called after game DLL
	GetEntityAPI2,	// pfnGetEntityAPI2				HL SDK2; called before game DLL
	NULL,			// pfnGetEntityAPI2_Post		META; called after game DLL
	NULL,			// pfnGetNewDLLFunctions		HL SDK2; called before game DLL
	NULL,			// pfnGetNewDLLFunctions_Post	META; called after game DLL
	GetEngineFunctions,	// pfnGetEngineFunctions	META; called before HL engine
	NULL,			// pfnGetEngineFunctions_Post	META; called after HL engine
};

// Description of plugin
plugin_info_t Plugin_info = {
	META_INTERFACE_VERSION,	// ifvers
	"Last Hope",	// name
	"0.1",	// version
	"2026/09/08",	// date
	"Progress",	// author
	"",	// url
	"LAST_HOPE",	// logtag, all caps please
	PT_ANYTIME,	// (when) loadable
	PT_ANYPAUSE,	// (when) unloadable
};

// Global vars from metamod:
meta_globals_t *gpMetaGlobals;		// metamod globals
gamedll_funcs_t *gpGamedllFuncs;	// gameDLL function tables
mutil_funcs_t *gpMetaUtilFuncs;		// metamod utility functions

// Metamod requesting info about this plugin:
//  ifvers			(given) interface_version metamod is using
//  pPlugInfo		(requested) struct with info about plugin
//  pMetaUtilFuncs	(given) table of utility functions provided by metamod
C_DLLEXPORT int Meta_Query(char * /*ifvers */, plugin_info_t **pPlugInfo,
		mutil_funcs_t *pMetaUtilFuncs) 
{
	// Give metamod our plugin_info struct
	*pPlugInfo=&Plugin_info;
	// Get metamod utility function table.
	gpMetaUtilFuncs=pMetaUtilFuncs;
	return(TRUE);
}

// Metamod attaching plugin to the server.
//  now				(given) current phase, ie during map, during changelevel, or at startup
//  pFunctionTable	(requested) table of function tables this plugin catches
//  pMGlobals		(given) global vars from metamod
//  pGamedllFuncs	(given) copy of function tables from game dll
C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME /* now */, 
		META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, 
		gamedll_funcs_t *pGamedllFuncs) 
{
	if (!pFunctionTable) 
		return(FALSE);

	if (!pMGlobals) 
		return(FALSE);

    if (!pGamedllFuncs)
        return(FALSE);
	
	gpMetaGlobals  = pMGlobals;
	gpGamedllFuncs = pGamedllFuncs;
	
	memcpy(
        pFunctionTable, 
        &gMetaFunctionTable, 
        sizeof(META_FUNCTIONS)
    );

    LH_LogInit();

    LH_START("===================================");
    LH_START("Last Hope %s attaching", Plugin_info.version);

    if (!Initialize())
    {
        LH_ERROR("[Meta_Attach()] ReGameDll initialization failed!");
        LH_LogShutdown();
        return(FALSE);
    }

    LH_INFO("ReGameDLL initialization successful");
    LH_INFO("Last Hope attached successfully");

	return(TRUE);
}

// Metamod detaching plugin from the server.
// now		(given) current phase, ie during map, etc
// reason	(given) why detaching (refresh, console unload, forced unload, etc)
C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME /* now */, 
		PL_UNLOAD_REASON /* reason */) 
{
    LH_START("Last Hope detaching");

    LH_LogShutdown();

	return(TRUE);
}
