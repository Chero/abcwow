#include <string>
#ifndef INSTANCE_SCRIPTS_SETUP_H
#define INSTANCE_SCRIPTS_SETUP_H

//Instances
void SetupArcatraz(ScriptMgr * mgr);
void SetupAuchenaiCrypts(ScriptMgr * mgr);
void SetupBlackrockSpire(ScriptMgr *mgr);
void SetupBlackrockDepths(ScriptMgr * mgr);
void SetupBloodFurnace(ScriptMgr * mgr);
void SetupBotanica(ScriptMgr * mgr);
void SetupDeadmines(ScriptMgr * mgr);
void SetupDireMaul(ScriptMgr * mgr);
void SetupHellfireRamparts(ScriptMgr * mgr);
void SetupManaTombs(ScriptMgr * mgr);
void SetupMaraudon(ScriptMgr * mgr);
//void SetupOldHillsbradFoothills(ScriptMgr * mgr);
void SetupRagefireChasm(ScriptMgr * mgr);
void SetupRazorfenDowns(ScriptMgr * mgr);
void SetupScarletMonastery(ScriptMgr * mgr);
void SetupScholomance(ScriptMgr * mgr);
void SetupSethekkHalls(ScriptMgr * mgr);
void SetupShadowfangKeep(ScriptMgr * mgr);
void SetupShadowLabyrinth(ScriptMgr * mgr);
void SetupTheBlackMorass(ScriptMgr * mgr);
void SetupTheMechanar(ScriptMgr * mgr);
void SetupTheShatteredHalls(ScriptMgr * mgr);
void SetupTheSlavePens(ScriptMgr * mgr);
void SetupTheSteamvault(ScriptMgr * mgr);
void SetupTheUnderbog(ScriptMgr * mgr);
void SetupUldaman(ScriptMgr * mgr);
void SetupTheStockade(ScriptMgr * mgr);
void SetupWailingCaverns(ScriptMgr * mgr);
//void SetupMagistersTerrace(ScriptMgr * mgr);
//Raids
void SetupBlackTemple(ScriptMgr * mgr);
void SetupBlackwingLair(ScriptMgr * mgr);
void SetupBattleOfMountHyjal(ScriptMgr * mgr);
void SetupGruulsLair(ScriptMgr * mgr);
void SetupKarazhan(ScriptMgr * mgr);
void SetupMoltenCore(ScriptMgr * mgr);
void SetupNaxxramas(ScriptMgr * mgr);
void SetupOnyxiasLair(ScriptMgr * mgr);
void SetupTheEye(ScriptMgr * mgr);
void SetupZulGurub(ScriptMgr * mgr);
void SetupSerpentshrineCavern(ScriptMgr * mgr);
void SetupMagtheridonsLair(ScriptMgr * mgr);
//void SetupSunwellPlateau(ScriptMgr* pScriptMgr);
void SetupWorldBosses(ScriptMgr * mgr);
void SetupZulAman(ScriptMgr * mgr);
//Events
//void SetupEventDarkPortal(ScriptMgr * mgr);
//other
//void SetupGenericAI(ScriptMgr * mgr);

struct SP_AI_Spell{
	SpellEntry *info;		// spell info
	char targettype;		// 0-self , 1-attaking target, ....
	bool instant;			// does it is instant or not?
	float perctrigger;		// % of the cast of this spell in a total of 100% of the attacks
	int attackstoptimer;	// stop the creature from attacking
	int soundid;			// sound id from DBC
	std::string speech;		// text displaied when spell was casted
	uint32 cooldown;		// spell cooldown
	uint32 casttime;		// "time" left to cast spell
	uint32 reqlvl;			// required level ? needed?
	float hpreqtocast;		// ? needed?
	float mindist2cast;		// min dist from caster to victim to perform cast (dist from caster >= mindist2cast)
	float maxdist2cast;		// max dist from caster to victim to perform cast (dist from caster <= maxdist2cast)
	int minhp2cast;			// min hp amount of victim to perform cast on it (health >= minhp2cast)
	int maxhp2cast;			// max hp amount of victim to perform cast on it (health <= maxhp2cast)
};

enum
{
	TARGET_SELF,
	TARGET_VARIOUS,
	TARGET_ATTACKING,
	TARGET_DESTINATION,
	TARGET_SOURCE,
	TARGET_RANDOM_FRIEND,	// doesn't work yet
	TARGET_RANDOM_SINGLE,
	TARGET_RANDOM_DESTINATION,

	//.....add
};

#endif
