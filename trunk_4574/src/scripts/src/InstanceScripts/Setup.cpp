#include "StdAfx.h"
#include "Setup.h"
#define SKIP_ALLOCATOR_SHARING 1
#include <ScriptSetup.h>

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	//Instances
	SetupArcatraz(mgr);
	SetupAuchenaiCrypts(mgr);
	SetupBlackrockDepths(mgr);
	SetupBlackrockSpire(mgr);
	SetupBloodFurnace(mgr);
	SetupBotanica(mgr);
	SetupDeadmines(mgr);
	SetupDireMaul(mgr);
	SetupHellfireRamparts(mgr);
	SetupManaTombs(mgr);
	SetupMaraudon(mgr);
	//SetupOldHillsbradFoothills(mgr);
	SetupRagefireChasm(mgr);
	SetupRazorfenDowns(mgr);
	SetupScarletMonastery(mgr);
	SetupScholomance(mgr);
	SetupSethekkHalls(mgr);
	SetupShadowfangKeep(mgr);
	SetupShadowLabyrinth(mgr);
	SetupTheBlackMorass(mgr);
	SetupTheMechanar(mgr);
	SetupTheShatteredHalls(mgr);
	SetupTheSlavePens(mgr);
	SetupTheSteamvault(mgr);
	SetupTheUnderbog(mgr);
	SetupUldaman(mgr);
	SetupTheStockade(mgr);
	SetupWailingCaverns(mgr);
	//SetupMagistersTerrace(mgr);
	//Raids
	SetupBlackTemple(mgr);
	SetupBlackwingLair(mgr);
	SetupBattleOfMountHyjal(mgr);
	SetupGruulsLair(mgr);
	SetupKarazhan(mgr);
	SetupMoltenCore(mgr);
	SetupNaxxramas(mgr);
	SetupOnyxiasLair(mgr);
	SetupTheEye(mgr);
	SetupZulGurub(mgr);
	SetupSerpentshrineCavern(mgr);
	SetupMagtheridonsLair(mgr);
	//SetupSunwellPlateau(mgr);
	SetupWorldBosses(mgr); 
	SetupZulAman(mgr);
	//Events
	//SetupEventDarkPortal(mgr);
	//Other
	//SetupGenericAI(mgr);
}

#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    return TRUE;
}

#endif
