#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Raid_SerpentShrineCavern.cpp Script									*/
/************************************************************************/

struct Coords
{
	float x;
	float y;
	float z;
	float o;
};

//Hydross

#define CN_HYDROSS 21216
#define CN_PURESPAWN 22035
#define CN_TAINTEDSPAWN 22036

//Water phase
#define HYDROSS_WATERTOMB 38235
#define HYDROSS_MARKOFHYDROSS1 38215
#define HYDROSS_MARKOFHYDROSS2 38216
#define HYDROSS_MARKOFHYDROSS3 38217
#define HYDROSS_MARKOFHYDROSS4 38218
#define HYDROSS_MARKOFHYDROSS5 38231
#define HYDROSS_MARKOFHYDROSS6 40584

//Nature phase
#define HYDROSS_VILESLUDGE 38246
#define HYDROSS_MARKOFCORRUPT1 38219
#define HYDROSS_MARKOFCORRUPT2 38220
#define HYDROSS_MARKOFCORRUPT3 38221
#define HYDROSS_MARKOFCORRUPT4 38222
#define HYDROSS_MARKOFCORRUPT5 38230
#define HYDROSS_MARKOFCORRUPT6 40583

class HYDROSSAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HYDROSSAI);

	HYDROSSAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		m_phase = 1;
		m_markcd = 15;
		m_markstate = 1;
		m_watertomb = 7;
		m_vilesludge = 15;
		m_enrage = 600;
		enraged = 0;
		_unit->SchoolImmunityList[4] = 1;
		_unit->BaseAttackType = 4;
	}
	void OnCombatStart(Unit* mTarget)
	{
		_unit->PlaySoundToSet(11289);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I cannot allow you to interfere!");
		RegisterAIUpdateEvent(1000);
		m_phase = 1;
		m_markcd = 15;
		m_markstate = 1;
		m_enrage = 600;
		enraged = 0;
		m_watertomb = 7;
		_unit->SchoolImmunityList[3] = 0;
		_unit->SchoolImmunityList[4] = 1;
		_unit->BaseAttackType = 4;
	}
	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive())
		{
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 20162);
			_unit->DamageDoneModPCT[0] = 0;
			_unit->DamageDoneModPCT[1] = 0;
			_unit->DamageDoneModPCT[2] = 0;
			_unit->DamageDoneModPCT[3] = 0;
			_unit->DamageDoneModPCT[4] = 0;
			_unit->DamageDoneModPCT[5] = 0;
			_unit->DamageDoneModPCT[6] = 0;
		}
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}
	void OnDied(Unit * mKiller)
	{
		switch (m_phase)
		{
		case 1:
			_unit->PlaySoundToSet(11293);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am... released...");
			break;
		case 2:
			_unit->PlaySoundToSet(11300);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are the disease, not I");
			break;
		}
		RemoveAIUpdateEvent();
		summon1->Despawn(100, 0);
		summon2->Despawn(100, 0);
		summon3->Despawn(100, 0);
		summon4->Despawn(100, 0);
	}
	void OnTargetDied(Unit* mTarget)
	{
		int val = RandomUInt(100)%2;
		switch (m_phase)
		{
		case 1:
			switch (val)
			{
			case 0:
				_unit->PlaySoundToSet(11291);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "They have forced me to this...");
				break;
			case 1:
				_unit->PlaySoundToSet(11292);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have no choice.");
				break;
			}
			break;
		case 2:
			switch (val)
			{
			case 0:
				_unit->PlaySoundToSet(11298);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will purge you from this place.");
				break;
			case 1:
				_unit->PlaySoundToSet(11299);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are no better than they!");
				break;
			}
			break;
		}
	}
	void AIUpdate()
	{
		m_enrage--;
		m_markcd--;
		if (!m_enrage && !enraged)
		{
			_unit->DamageDoneModPCT[0] = 2;
			_unit->DamageDoneModPCT[1] = 2;
			_unit->DamageDoneModPCT[2] = 2;
			_unit->DamageDoneModPCT[3] = 2;
			_unit->DamageDoneModPCT[4] = 2;
			_unit->DamageDoneModPCT[5] = 2;
			_unit->DamageDoneModPCT[6] = 2;
			enraged = 1;
		}
		uint32 dist = FL2UINT(_unit->GetAIInterface()->_CalcDistanceFromHome());
		if (dist > 700)
		{
			if (m_phase == 1)
			{
				m_phase = 2;
				_unit->PlaySoundToSet(11297);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Aaghh, the poison...");
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 20609);
				_unit->GetAIInterface()->WipeHateList();
				m_markcd = 15;
				m_markstate = 1;
				m_vilesludge = 15;

				summon1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDSPAWN, _unit->GetPositionX() - 5, _unit ->GetPositionY() - 5, _unit->GetPositionZ(), 90, true, false, 0, 0);
				summon2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDSPAWN, _unit->GetPositionX() - 5, _unit ->GetPositionY() + 5, _unit->GetPositionZ(), 90, true, false, 0, 0);
				summon3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDSPAWN, _unit->GetPositionX() + 5, _unit ->GetPositionY() - 5, _unit->GetPositionZ(), 90, true, false, 0, 0);
				summon4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDSPAWN, _unit->GetPositionX() + 5, _unit ->GetPositionY() + 5, _unit->GetPositionZ(), 90, true, false, 0, 0);
				_unit->SchoolImmunityList[4] = 0;
				_unit->SchoolImmunityList[3] = 1;
				_unit->BaseAttackType = 3;
			}
			else 
			{
				if (!m_markcd)
				{
					switch (m_markstate)
					{
					case 1:
						_unit->CastSpell(_unit, HYDROSS_MARKOFCORRUPT1, true);
						break;
					case 2:
						_unit->CastSpell(_unit, HYDROSS_MARKOFCORRUPT2, true);
						break;
					case 3:
						_unit->CastSpell(_unit, HYDROSS_MARKOFCORRUPT3, true);
						break;
					case 4:
						_unit->CastSpell(_unit, HYDROSS_MARKOFCORRUPT4, true);
						break;
					case 5:
						_unit->CastSpell(_unit, HYDROSS_MARKOFCORRUPT5, true);
						break;
					case 6:
						_unit->CastSpell(_unit, HYDROSS_MARKOFCORRUPT6, true);
						break;
					default:
						_unit->CastSpell(_unit, HYDROSS_MARKOFCORRUPT6, true);
						break;
					}
					m_markstate++;
					m_markcd = 15;
				}
				m_vilesludge--;
				if (!m_vilesludge)
				{
					Unit *target = RandomTarget(false, true, 10000);
					if (target)
					{
						_unit->CastSpell(target, HYDROSS_VILESLUDGE, false);
					}
					m_vilesludge = 15;
				}
			}
		}
		else
		{
			if (m_phase == 2)
			{
				m_phase = 1;
				_unit->PlaySoundToSet(11290);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Better, much better.");
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 20162);
				_unit->GetAIInterface()->WipeHateList();
				m_markcd = 15;
				m_markstate = 1;
				m_watertomb = 7;

				summon1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_PURESPAWN, _unit->GetPositionX() - 5, _unit ->GetPositionY() - 5, _unit->GetPositionZ(), 90, true, false, 0, 0);
				summon2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_PURESPAWN, _unit->GetPositionX() - 5, _unit ->GetPositionY() + 5, _unit->GetPositionZ(), 90, true, false, 0, 0);
				summon3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_PURESPAWN, _unit->GetPositionX() + 5, _unit ->GetPositionY() - 5, _unit->GetPositionZ(), 90, true, false, 0, 0);
				summon4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_PURESPAWN, _unit->GetPositionX() + 5, _unit ->GetPositionY() + 5, _unit->GetPositionZ(), 90, true, false, 0, 0);
				_unit->SchoolImmunityList[4] = 1;
				_unit->SchoolImmunityList[3] = 0;
				_unit->BaseAttackType = 4;
			}
			else
			{
				if (!m_markcd)
				{
					switch (m_markstate)
					{
					case 1:
						_unit->CastSpell(_unit, HYDROSS_MARKOFHYDROSS1, true);
						break;
					case 2:
						_unit->CastSpell(_unit, HYDROSS_MARKOFHYDROSS2, true);
						break;
					case 3:
						_unit->CastSpell(_unit, HYDROSS_MARKOFHYDROSS3, true);
						break;
					case 4:
						_unit->CastSpell(_unit, HYDROSS_MARKOFHYDROSS4, true);
						break;
					case 5:
						_unit->CastSpell(_unit, HYDROSS_MARKOFHYDROSS5, true);
						break;
					case 6:
						_unit->CastSpell(_unit, HYDROSS_MARKOFHYDROSS6, true);
						break;
					default:
						_unit->CastSpell(_unit, HYDROSS_MARKOFHYDROSS6, true);
						break;
					}
					m_markstate++;
					m_markcd = 15;
				}
				m_watertomb--;
				if (!m_watertomb)
				{
					Unit *target = RandomTarget(false, true, 10000);
					if (target)
						_unit->CastSpell(target, HYDROSS_WATERTOMB, false);
					m_watertomb = 7;
				}
			}
		}
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}
protected:
	uint32 m_phase;
	uint32 m_markcd;
	uint32 m_markstate;
	uint32 m_watertomb;
	uint32 m_vilesludge;
	uint32 m_enrage;
	uint32 enraged;

	Creature *summon1;
	Creature *summon2;
	Creature *summon3;
	Creature *summon4;
};

//Pure Spawn of Hydross

#define CN_PURESPAWN 22035

class HYDROSSPURESPAWNAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HYDROSSPURESPAWNAI);

	HYDROSSPURESPAWNAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->SchoolImmunityList[4] = 1;
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
	}
protected:
};

//Tainted Spawn of Hydross

#define CN_TAINTEDSPAWN 22036

class HYDROSSTAINTEDSPAWNAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HYDROSSTAINTEDSPAWNAI);

	HYDROSSTAINTEDSPAWNAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->SchoolImmunityList[3] = 1;
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
	}
protected:
};

//The Lurker Below

#define CN_LURKERBELOW 21217
#define CN_COILFANGAMBUSHER 21865
#define CN_COILFANGGUARDIAN 21873

#define LURKER_WATERBOLT 37138
#define LURKER_WHIRL 37363
#define LURKER_GEYSER 37478
#define LURKER_SPOUT 37431

static Coords lurkerbelow[] = 
{
	{ 0, 0, 0, 0 },
	{ 37.918114f, -416.081146f, -19.896648f, 0.061985f }
};

class LURKERBELOWAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LURKERBELOWAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	LURKERBELOWAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{

		nrspells = 1;
		for(int i=0; i<nrspells; i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(LURKER_GEYSER);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 17;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, FLY));

		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		_unit->GetAIInterface()->setWaypointToMove(1);

		_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
		_unit->GetAIInterface()->m_moveFly = true;
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		submergetimer = 90;
		submerged = 0;
		whirlcd = 18;
		waterboltcd = 0;
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
		RegisterAIUpdateEvent(1000);
		submergetimer = 90;
		submerged = 0;
		whirlcd = 18;
		waterboltcd = 0;
		CastTime();
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive())
		{
			_unit->Emote(EMOTE_ONESHOT_SUBMERGE);
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
		}
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		if (!submerged)
		{
			if (!submergetimer)
			{
				_unit->RemoveAllAuras();
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				_unit->GetAIInterface()->disable_melee = true;
				_unit->Emote(EMOTE_ONESHOT_SUBMERGE);
				_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
				submergetimer = 60;
				submerged = 1;

				Creature *summon;
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGAMBUSHER, 12.239388f, -454.372284f, -19.793467f, 1.132437f, true, false, 0, 0);
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGAMBUSHER, 48.335369f, -458.581665f, -19.793333f, 1.807879f, true, false, 0, 0);
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGAMBUSHER, 62.277702f, -380.135712f, -19.721540f, 4.036446f, true, false, 0, 0);

				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGGUARDIAN, 38.281582f, -392.905212f, -18.703152f, 5.200795f, true, false, 0, 0);
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGGUARDIAN, 19.376112f, -425.276642f, -19.497267f, 5.764311f, true, false, 0, 0);
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGGUARDIAN, 50.974178f, -436.504120f, -19.012314f, 1.949248f, true, false, 0, 0);
			}
			else
			{
				if (!whirlcd)
				{
					_unit->Emote(EMOTE_STATE_WHIRLWIND);
					_unit->CastSpell(_unit, LURKER_WHIRL, true);
					whirlcd = 18;
				}
				else
				{
					whirlcd--;
				}
				if (_unit->GetAIInterface()->GetMostHated() && (_unit->GetDistance2dSq(_unit->GetAIInterface()->GetMostHated()) > 450))
				{
					if (GetPlayerCount() == 0)
					{
						if (waterboltcd >= 5)
						{
							if (_unit->GetCurrentSpell() == NULL)
							{
								Unit *target = RandomTarget(true, true, 40000);
								if (target)
								{
									_unit->GetAIInterface()->SetNextTarget(target);
									_unit->CastSpell(target, LURKER_WATERBOLT, false);
								}
							}
						}
						else
							waterboltcd++;
					}
					else
					{
						uint32 threat = _unit->GetAIInterface()->getThreatByPtr(_unit->GetAIInterface()->GetMostHated());
						_unit->GetAIInterface()->RemoveThreatByPtr(_unit->GetAIInterface()->GetMostHated());
						Unit * target = RandomTarget(false, true, 450);
						if (target)
						{
							_unit->GetAIInterface()->modThreatByPtr(target, threat);
							_unit->GetAIInterface()->AttackReaction(target, 0, 0);
							waterboltcd = 0;
						}
					}
				}
				else
					_unit->GetAIInterface()->modThreatByPtr(_unit->GetAIInterface()->GetNextTarget(), 100000);
				float val = (float)RandomFloat(100.0f);
				SpellCast(val);
				submergetimer--;
			}
		}
		else
		{
			if (!submergetimer)
			{
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
				_unit->GetAIInterface()->disable_melee = false;
				submergetimer = 90;
				submerged = 0;
				whirlcd = 18;
			}
			else
				submergetimer--;
		}
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->GetAIInterface()->SetNextTarget(target);
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

	int GetPlayerCount()
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return 0;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if ((temp->GetTypeId() == TYPEID_PLAYER) && (_unit->GetDistance2dSq(temp) <= 500))
			{
				targetTable.push_back(temp);
			}
		}
		if (!targetTable.size())
			return 0;

		return (int)targetTable.size();
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->m_canMove = false;
	}
	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = lurkerbelow[id].x;
		wp->y = lurkerbelow[id].y;
		wp->z = lurkerbelow[id].z;
		wp->o = lurkerbelow[id].o;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}
protected:
	int nrspells;
	int submergetimer;
	int submerged;
	int whirlcd;
	int waterboltcd;
};

// COILFANG AMBUSHER AI

#define CN_COILFANGAMBUSHER 21865

#define COILFANGAMBUSHER_MULTISHOT 29576


class COILFANGAMBUSHERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGAMBUSHERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];


	COILFANGAMBUSHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(COILFANGAMBUSHER_MULTISHOT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 4500;

		_unit->GetAIInterface()->setCurrentAgent(AGENT_RANGED);
	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	int nrspells;
};

// COILFANG GUARDIAN AI

#define CN_COILFANGGUARDIAN 21873

#define COILFANGGUARDIAN_ARCINGSMASH 28168
#define COILFANGGUARDIAN_HAMSTRING 29667

class COILFANGGUARDIANAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGGUARDIANAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

	COILFANGGUARDIANAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(COILFANGGUARDIAN_ARCINGSMASH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(COILFANGGUARDIAN_HAMSTRING);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;


	}

	void OnCombatStart(Unit* mTarget)
	{
		CastTime();
		RegisterAIUpdateEvent(1000);
	}

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	int nrspells;
};


//TODO: Emotes for bossmods
//Morogrim Tidewalker

#define CN_MOROGRIM 21213
#define CN_MURLOC 21920
#define CN_WATERGLOBULE 21913
#define MOROGRIM_TIDALWAVE 37730
#define MOROGRIM_WATERYGRAVE 38049
#define MOROGRIM_EARTHQUAKE 37764
//#define MOROGRIM_WATERGLOBULE 37854

class MOROGRIMAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MOROGRIMAI);
	bool m_spellcheck[1];
	SP_AI_Spell spells[1];

	MOROGRIMAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0; i<nrspells ;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(MOROGRIM_TIDALWAVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 2000;

		m_phase = 1;
		earthquakecd = 45 + RandomUInt(100)%15;
		waterygravecd = 25 + RandomUInt(100)%10;
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->PlaySoundToSet(11321);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Flood of the deep, take you!");
		earthquakecd = 45 + RandomUInt(100)%15;
		waterygravecd = 25 + RandomUInt(100)%10;
		m_phase = 1;
		CastTime();
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(11329);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Great... currents of... Ageon.");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		int val = RandomUInt(100)%3;
		switch (val)
		{
		case 0:
			_unit->PlaySoundToSet(11328);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Only the strong survive.");
			break;
		case 1:
			_unit->PlaySoundToSet(11327);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Struggling only makes it worse.");
			break;
		case 2:
			_unit->PlaySoundToSet(11326);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is done!");
			break;
		}
	}
	void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 25 && m_phase == 1)
		{
			m_phase = 2;
			waterglobulecd = 25 + RandomUInt(100)%10;
		}
		switch (m_phase)
		{
		case 1:
			PhaseOne();
			break;
		case 2:
			PhaseTwo();
			break;
		}
		if (!earthquakecd)
		{
			_unit->CastSpell(_unit, MOROGRIM_EARTHQUAKE, true);
			int val = RandomUInt(100)%2;
			switch (val)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "By the Tides, kill them at once!");
				_unit->PlaySoundToSet(11322);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Destroy them my subjects!");
				_unit->PlaySoundToSet(11323);
				break;
			}
			Unit *tank = _unit->GetAIInterface()->GetMostHated();
			Creature *murloc;
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 513.671875f, -724.752747f, -7.144188f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 513.671875f, -724.752747f, -7.144188f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 513.671875f, -724.752747f, -7.144188f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 513.671875f, -724.752747f, -7.144188f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 513.671875f, -724.752747f, -7.144188f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 513.671875f, -724.752747f, -7.144188f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 259.144714f, -724.210083f, -3.336553f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 259.144714f, -724.210083f, -3.336553f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 259.144714f, -724.210083f, -3.336553f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 259.144714f, -724.210083f, -3.336553f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 259.144714f, -724.210083f, -3.336553f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			murloc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MURLOC, 259.144714f, -724.210083f, -3.336553f, 0, true, false, 0, 0);
			murloc->GetAIInterface()->_CalcDestinationAndMove(_unit, 5);
			murloc->GetAIInterface()->AttackReaction(tank, 1000, 0);
			earthquakecd = 45 + RandomUInt(100)%15;
		}
		else
		{
			earthquakecd--;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				spells[i].casttime--;

				if (m_spellcheck[i])
				{
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}



	void PhaseOne()
	{
		if (!waterygravecd)
		{
			if (GetPlayerCount() >= 5)
			{
				Player *target1 = (Player *) RandomTarget(false, true, 10000);
				Player *target2 = (Player *) RandomTarget(false, true, 10000);
				while(target2 == target1)
				{
					target2 = (Player *) RandomTarget(false, true, 10000);
				}
				Player *target3 = (Player *) RandomTarget(false, true, 10000);
				while (target3 == target1 || target3 == target2)
				{
					target3 = (Player *) RandomTarget(false, true, 10000);
				}
				Player *target4 = (Player *) RandomTarget(false, true, 10000);
				while (target4 == target1 || target4 == target2 || target4 == target3)
				{
					target4 = (Player *) RandomTarget(false, true, 10000);
				}
				target1->_Relocate(_unit->GetMapId(), LocationVector(366.443512f, -708.822388f, -14.357947f, target1->GetOrientation()), false, false, _unit->GetInstanceID());
				_unit->CastSpell(target1, MOROGRIM_WATERYGRAVE, true);
				target2->_Relocate(_unit->GetMapId(), LocationVector(373.805511f, -691.146116f, -14.446006f, target2->GetOrientation()), false, false, _unit->GetInstanceID());
				_unit->CastSpell(target2, MOROGRIM_WATERYGRAVE, true);
				target3->_Relocate(_unit->GetMapId(), LocationVector(365.522644f, -737.217712f, -14.444579f, target3->GetOrientation()), false, false, _unit->GetInstanceID());
				_unit->CastSpell(target3, MOROGRIM_WATERYGRAVE, true);
				target4->_Relocate(_unit->GetMapId(), LocationVector(337.470581f, -732.931885f, -14.173863f, target4->GetOrientation()), false, false, _unit->GetInstanceID());
				_unit->CastSpell(target4, MOROGRIM_WATERYGRAVE, true);
			}
			waterygravecd = 25 + RandomUInt(100)%10;
		}
		else
		{
			waterygravecd--;
		}
	}
	void PhaseTwo()
	{
		if (!waterglobulecd)
		{
			if (GetPlayerCount() >= 4)
			{
				int val = RandomUInt(100)%2;
				switch (val)
				{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It will soon be over.");
					_unit->PlaySoundToSet(11325);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "There is nowhere to hide!");
					_unit->PlaySoundToSet(11324);
					break;
				}
				Player *target1 = (Player *) RandomTarget(true, true, 10000);
				Player *target2 = (Player *) RandomTarget(true, true, 10000);
				while(target2 == target1)
				{
					target2 = (Player *) RandomTarget(true, true, 10000);
				}
				Player *target3 = (Player *) RandomTarget(true, true, 10000);
				while (target3 == target1 || target3 == target2)
				{
					target3 = (Player *) RandomTarget(true, true, 10000);
				}
				Player *target4 = (Player *) RandomTarget(true, true, 10000);
				while (target4 == target1 || target4 == target2 || target4 == target3)
				{
					target4 = (Player *) RandomTarget(true, true, 10000);
				}
				Creature *summon1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_WATERGLOBULE, 366.443512f, -708.822388f, -14.357947f, 0, true, false, 0, 0);
				summon1->GetAIInterface()->AttackReaction(target1, 100000, 0);
				Creature *summon2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_WATERGLOBULE, 373.805511f, -691.146116f, -14.446006f, 0, true, false, 0, 0);
				summon2->GetAIInterface()->AttackReaction(target2, 100000, 0);
				Creature *summon3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_WATERGLOBULE, 365.522644f, -737.217712f, -14.444579f, 0, true, false, 0, 0);
				summon3->GetAIInterface()->AttackReaction(target3, 100000, 0);
				Creature *summon4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_WATERGLOBULE, 337.470581f, -732.931885f, -14.173863f, 0, true, false, 0, 0);
				summon4->GetAIInterface()->AttackReaction(target4, 100000, 0);
			}
			waterglobulecd = 25 + RandomUInt(100)%10;
		}
		else
		{
			waterglobulecd--;
		}
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

	int GetPlayerCount()
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return 0;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (temp->GetTypeId() == TYPEID_PLAYER)
			{
				targetTable.push_back(temp);
			}
		}
		if (!targetTable.size())
			return 0;

		return (int)targetTable.size();
	}

protected:
	int nrspells;
	uint32 m_phase;
	uint32 waterygravecd;
	uint32 waterglobulecd;
	uint32 earthquakecd;
};

//Water Globule

#define CN_WATERGLOBULE 21913

class WATERGLOBULEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WATERGLOBULEAI);

	WATERGLOBULEAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		//_unit->SetUInt64Value(UNIT_FIELD_FLAGS, U_FIELD_FLAG_MAKE_CHAR_UNTOUCHABLE);
		_unit->SchoolImmunityList[0] = 1;
		_unit->SchoolImmunityList[1] = 1;
		_unit->SchoolImmunityList[2] = 1;
		_unit->SchoolImmunityList[3] = 1;
		_unit->SchoolImmunityList[4] = 1;
		_unit->SchoolImmunityList[5] = 1;
		_unit->SchoolImmunityList[6] = 1;
		_unit->Despawn(15000, 0);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
	}
	void AIUpdate()
	{
	}
protected:
};

//Tidewalker Lurker

#define CN_MURLOC 21920

class MOROGRIMMURLOCAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MOROGRIMMURLOCAI);

	MOROGRIMMURLOCAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
	}
protected:
};

// Fathom Lord Karathress

#define CN_KARATHRESS 21214
#define CN_FATHOMGUARDSHARKKIS 21966
#define CN_FATHOMGUARDCARIBDIS 21964
#define CN_FATHOMGUARDTIDALVESS 21965
#define CN_SPITFIRE 22091

#define KARATHRESS_CATACLYSMICBOLT 38441
#define KARATHRESS_TIDALSURGE 38357
#define KARATHRESS_BEASTWITHIN 38373

class KARATHRESSAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KARATHRESSAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	KARATHRESSAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0; i<nrspells; i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(KARATHRESS_CATACLYSMICBOLT);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		m_eventstarted = false;
		sharkkisalive = true;
		caribdisalive = true;
		tidalvessalive = true;
		caribdis = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FATHOMGUARDCARIBDIS, 459.759521f, -544.832397f, -7.547507f, 1.845423f, true, false, 0, 0);
		sharkkis = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FATHOMGUARDSHARKKIS, 462.492615f, -540.305237f, -7.547507f, 3.143288f, true, false, 0, 0);
		tidalvess = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FATHOMGUARDTIDALVESS, 459.759521f, -536.210449f, -7.547507f, 4.197681f, true, false, 0, 0);
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->PlaySoundToSet(11277);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Guards, attention! We have visitors....");
		EventStart(mTarget);
		RegisterAIUpdateEvent(1000);
		CastTime();
	}
	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive())
		{
			caribdis->Despawn(100, 0);
			sharkkis->Despawn(100, 0);
			tidalvess->Despawn(100, 0);
			_unit->Despawn(100, 2500);
		}
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(22820, 450.574066f, -546.205322f, -7.546521f, 0.352241f, true, false, 0, 0);
		_unit->PlaySoundToSet(11285);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Her ... excellency ... awaits!");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		_unit->PlaySoundToSet(11284);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am rid of you.");
	}

	void AIUpdate()
	{
		if (!caribdis->isAlive() && caribdisalive)
		{
			caribdisalive = false;
			randomspeech();
			tidalsurgecd = 15;
		}
		if (!sharkkis->isAlive() && sharkkisalive)
		{
			sharkkisalive = false;
			randomspeech();
			beastwithincd = 15;
		}
		if (!tidalvess->isAlive() && tidalvessalive)
		{
			tidalvessalive = false;
			randomspeech();
			firetotemcd = 15;
		}
		if (!caribdisalive)
		{
			if (!tidalsurgecd)
			{
				tidalsurge();
				tidalsurgecd = 15 + RandomUInt(100)%3;
			}
			else
			{
				tidalsurgecd--;
			}
		}
		if (!sharkkisalive)
		{
			if (!beastwithincd)
			{
				_unit->CastSpell(_unit, KARATHRESS_BEASTWITHIN, true);
				beastwithincd = 60;
			}
			else
			{
				beastwithincd--;
			}
		}
		if (!tidalvessalive)
		{
			if (!firetotemcd)
			{
				Creature *totem = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SPITFIRE, _unit->GetPositionX() + 2, _unit->GetPositionY() + 2, _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
				firetotemcd = 85;
			}
			else
			{
				firetotemcd--;
			}
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, false, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->GetAIInterface()->SetNextTarget(target);
							spells[i].info->dmg_bonus = (target->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/2);
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

	void randomspeech()
	{
		int val = RandomUInt(100)%3;
		switch (val)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "More knowledge, more power!");
			_unit->PlaySoundToSet(11281);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am more powerful than ever!");
			_unit->PlaySoundToSet(11280);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Go on, kill them! I'll be the better for it!");
			_unit->PlaySoundToSet(11279);
			break;
		}
	}
	void EventStart(Unit* mTarget)
	{
		if (!m_eventstarted)
		{
			m_eventstarted = true;
			_unit->GetAIInterface()->AttackReaction(mTarget, 0, 0);
			caribdis->GetAIInterface()->AttackReaction(mTarget, 0, 0);
			sharkkis->GetAIInterface()->AttackReaction(mTarget, 0, 0);
			tidalvess->GetAIInterface()->AttackReaction(mTarget, 0, 0);
		}
	}

	void tidalsurge()
	{
		if (_unit->GetAIInterface()->getAITargetsCount() > 0)
		{
			TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
			TargetMap::iterator itr;
			for (itr = targets->begin(); itr != targets->end();itr++)
			{
				if (_unit->GetDistance2dSq(itr->first) <= 100)
				{
					itr->first->CastSpell(itr->first, KARATHRESS_TIDALSURGE, true);
				}
			}
		}
	}

protected:
	int nrspells;
	Creature *sharkkis;
	Creature *caribdis;
	Creature *tidalvess;
	bool sharkkisalive;
	bool caribdisalive;
	bool tidalvessalive;
	bool m_eventstarted;
	uint32 tidalsurgecd;
	uint32 beastwithincd;
	uint32 firetotemcd;
};

// Caribdis

#define CN_KARATHRESS 21214
#define CN_FATHOMGUARDCARIBDIS 21964

#define CARIBDIS_WATERBOLTVOLLEY 38335
#define CARIBDIS_TIDALSURGE 38357

class CARIBDISAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CARIBDISAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	CARIBDISAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		karathress = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(456.138336f, -541.419678f, -7.547507f, CN_KARATHRESS);
		nrspells = 1;
		for(int i=0; i<nrspells; i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(CARIBDIS_WATERBOLTVOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 30;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		karathress = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(456.138336f, -541.419678f, -7.547507f, CN_KARATHRESS);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
		if (karathress)
		{
			CreatureAIScript *mob_script = karathress->GetScript();
			((KARATHRESSAI*)mob_script)->EventStart(mTarget);
		}
		CastTime();
		tidalsurgecd = 15 + RandomUInt(100)%3;
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		if (!tidalsurgecd)
		{
			tidalsurge();
			tidalsurgecd = 15 + RandomUInt(100)%3;
		}
		else
		{
			tidalsurgecd--;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void tidalsurge()
	{
		if (_unit->GetAIInterface()->getAITargetsCount() > 0)
		{
			TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
			TargetMap::iterator itr;
			for (itr = targets->begin(); itr != targets->end();itr++)
			{
				if (_unit->GetDistance2dSq(itr->first) <= 100)
				{
					itr->first->CastSpell(itr->first, CARIBDIS_TIDALSURGE, true);
				}
			}
		}
	}

protected:
	Creature *karathress;
	int nrspells;
	uint32 tidalsurgecd;
};

// Sharkkis

#define CN_KARATHRESS 21214
#define CN_FATHOMGUARDSHARKKIS 21966

#define CN_FATHOMLURKER 22119
#define CN_FATHOMSPOREBAT 22120

#define SHARKKIS_VIPERSTING 39413
#define SHARKKIS_BEASTWITHIN 38373
#define SHARKKIS_MULTISHOT 34879

class SHARKKISAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SHARKKISAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

	SHARKKISAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		karathress = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(456.138336f, -541.419678f, -7.547507f, CN_KARATHRESS);
		nrspells = 2;
		for(int i=0; i<nrspells; i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(SHARKKIS_VIPERSTING);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(SHARKKIS_MULTISHOT);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

		karathress = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(456.138336f, -541.419678f, -7.547507f, CN_KARATHRESS);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
		if (karathress)
		{
			CreatureAIScript *mob_script = karathress->GetScript();
			((KARATHRESSAI*)mob_script)->EventStart(mTarget);
		}
		CastTime();
		petcd = 15;
		beastwithincd = 35;
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		if (!petcd)
		{
			if (!pet)
			{
				int val = RandomUInt(100)%2;
				switch (val)
				{
				case 0:
					pet = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FATHOMLURKER, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
					break;
				case 1:
					pet = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FATHOMSPOREBAT, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
					break;
				}
			}
			else
			{
				if (!pet->isAlive())
				{
					int val = RandomUInt(100)%2;
					switch (val)
					{
					case 0:
						pet = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FATHOMLURKER, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
						break;
					case 1:
						pet = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FATHOMLURKER, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
						break;
					}
				}
			}
			petcd = 60;
		}
		else
		{
			petcd--;
		}
		if (!beastwithincd)
		{
			_unit->CastSpell(_unit, SHARKKIS_BEASTWITHIN, true);
			if (pet && pet->isAlive())
				pet->CastSpell(pet, SHARKKIS_BEASTWITHIN, true); 
			beastwithincd = 60;
		}
		else
		{
			beastwithincd--;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, false, spells[i].info->base_range_or_radius_sqr);
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

protected:
	Creature *karathress;
	int nrspells;
	uint32 petcd;
	uint32 beastwithincd;
	Creature *pet;
};

//Sharkkis Pets

#define CN_FATHOMLURKER 22119
#define CN_FATHOMSPOREBAT 22120

class SHARKISSPETSAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SHARKISSPETSAI);

	SHARKISSPETSAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
	}

protected:
};

// Tidalvess

#define CN_KARATHRESS 21214
#define CN_FATHOMGUARDTIDALVESS 21965

#define CN_SPITFIRE 22091
#define CN_EARTHBIND 22486
#define TIDALVESS_WINDFURY 38229
#define TIDALVESS_FROSTSHOCK 29666
#define TIDALVESS_HEALINGWAVE 38330

class TIDALVESSAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TIDALVESSAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

	TIDALVESSAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		karathress = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(456.138336f, -541.419678f, -7.547507f, CN_KARATHRESS);
		nrspells = 3;
		for(int i=0; i<nrspells; i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(TIDALVESS_WINDFURY);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 25;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(TIDALVESS_FROSTSHOCK);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 12;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(TIDALVESS_HEALINGWAVE);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].cooldown = 20;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;

		karathress = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(456.138336f, -541.419678f, -7.547507f, CN_KARATHRESS);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
		if (karathress)
		{
			CreatureAIScript *mob_script = karathress->GetScript();
			((KARATHRESSAI*)mob_script)->EventStart(mTarget);
		}
		earthtotemcd = 15;
		firetotemcd = 25;
		CastTime();
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		if (!earthtotemcd)
		{
			Creature *totem = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_EARTHBIND, _unit->GetPositionX() + 2, _unit->GetPositionY() + 2, _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			earthtotemcd = 55;
		}
		else
		{
			earthtotemcd--;
		}
		if (!firetotemcd)
		{
			Creature *totem = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SPITFIRE, _unit->GetPositionX() - 2, _unit->GetPositionY() - 2, _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			firetotemcd = 85;
		}
		else
		{
			firetotemcd--;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

protected:
	Creature *karathress;
	int nrspells;
	uint32 firetotemcd;
	uint32 earthtotemcd;
};

//Spitfire Totem

#define CN_SPITFIRE 22091
#define SPITFIRE 38296

class SPITFIRETOTEMAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SPITFIRETOTEMAI);

	SPITFIRETOTEMAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		RegisterAIUpdateEvent(2200);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->Despawn(60000, 0);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(2200);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		_unit->CastSpell(_unit, SPITFIRE, false);
	}
protected:
};

//Earthbind Totem

#define CN_EARTHBIND 22486

#define EARTHBIND 3600

class EARTHBINDTOTEMAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(EARTHBINDTOTEMAI);

	EARTHBINDTOTEMAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		RegisterAIUpdateEvent(3000);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->Despawn(45000, 0);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(3000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		_unit->CastSpell(_unit, EARTHBIND, true);
	}

protected:
};

// Leotheras

#define CN_LEOTHERAS 21215
#define CN_LEOTHERASSHADOW 21875
#define CN_INNERDEMON 21857
#define CN_SPELLBINDER 21806

#define LEOTHERAS_BANISH 31797
#define LEOTHERAS_WHIRLWIND 37640
#define LEOTHERAS_CHAOSBLAST 37674
#define LEOTHERAS_INSIDIOUSWHISPER 37676

class LEOTHERASAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LEOTHERASAI);

	LEOTHERASAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		m_phase = 0;
		whirlwindcd = 16;
		whirlwinding = 0;
		phasecd = 60;
		m_enrage = 600;
		enraged = 0;
		shadow = NULL;
		m_eventstarted = false;
		_unit->CastSpell(_unit, LEOTHERAS_BANISH, true);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

		channeler1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SPELLBINDER, 378.073700f, -444.240204f, 29.524496f, 5.570440f, true, false, 0, 0);
		channeler2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SPELLBINDER, 382.517914f, -448.232117f, 29.527136f, 2.161806f, true, false, 0, 0);
		channeler3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SPELLBINDER, 383.528076f, -443.769165f, 29.527136f, 3.827633f, true, false, 0, 0);

		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStart(Unit* mTarget)
	{
		m_phase = 1;
		_unit->PlaySoundToSet(11312);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Finally, my banishment ends!");
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive())
		{
			if (shadow != NULL)
			{
				shadow->Despawn(0, 0);
				shadow = NULL;
			}
			channeler1->Despawn(100, 0);
			channeler2->Despawn(100, 0);
			channeler3->Despawn(100, 0);
			_unit->Despawn(100, 2500);
		}
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(11317);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You cannot kill me! Fools, I'll be back! I'll... aarghh...");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		int val = RandomUInt(100)%3;
		switch (m_phase)
		{
		case 1:
			switch (val)
			{
			case 0:
				_unit->PlaySoundToSet(11314);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Kill! KILL!");
				break;
			case 1:
				_unit->PlaySoundToSet(11315);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "That's right! Yes!");
				break;
			case 2:
				_unit->PlaySoundToSet(11316);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who's the master now?");
				break;
			}
			break;
		case 2:
			switch (val)
			{
			case 0:
				_unit->PlaySoundToSet(11306);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have no equal.");
				break;
			case 1:
				_unit->PlaySoundToSet(11307);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Perish, mortal.");
				break;
			case 2:
				_unit->PlaySoundToSet(11308);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes, YES! Ahahah!");
				break;
			}
			break;
		}
	}
	void AIUpdate()
	{
		if (!m_phase)
		{
			if (!channeler1->isAlive() && !channeler2->isAlive() && !channeler3->isAlive())
			{
				m_phase = 1;
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				_unit->RemoveAura(LEOTHERAS_BANISH);
				_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			}
		}
		else
		{
			if (!m_enrage && !enraged)
			{
				_unit->DamageDoneModPCT[0] = 2;
				_unit->DamageDoneModPCT[1] = 2;
				_unit->DamageDoneModPCT[2] = 2;
				_unit->DamageDoneModPCT[3] = 2;
				_unit->DamageDoneModPCT[4] = 2;
				_unit->DamageDoneModPCT[5] = 2;
				_unit->DamageDoneModPCT[6] = 2;
				enraged = 1;
			}
			else
				m_enrage--;
			if (_unit->GetHealthPct() <= 15 && m_phase < 3)
			{
				if (_unit->GetCurrentSpell())
				{
					_unit->GetCurrentSpell()->cancel();
				}
				_unit->PlaySoundToSet(11313);
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No... no! What have you done? I am the master! Do you hear me? I am... aaggh! Can't... contain him");
				_unit->RemoveAura(LEOTHERAS_WHIRLWIND);
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 20514);
				_unit->GetAIInterface()->disable_melee = false;
				_unit->setAttackTimer(5000, false);
				m_phase = 3;
				whirlwindcd = 16;
				shadow = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_LEOTHERASSHADOW,_unit->GetPositionX(), _unit->GetPositionY(),_unit->GetPositionZ(), _unit->GetOrientation(),true, false, 0, 0);
			}
			switch (m_phase)
			{
			case 1:
				PhaseOne();
				break;
			case 2:
				PhaseTwo();
				break;
			case 3:
				PhaseThree();
				break;
			}
		}
	}
	void PhaseOne()
	{
		if (!phasecd)
		{
			m_phase = 2;
			_unit->RemoveAura(LEOTHERAS_WHIRLWIND);
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 20125);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->WipeHateList();
			_unit->PlaySoundToSet(11304);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Be gone, trifling elf. I am in control now.");
			phasecd = 60;
			chaosblastcd = 0;
			innerdemonscd = 20;
			innerdemons = 0;
		}
		else 
		{
			if (!whirlwinding)
			{
				whirlwindcd--;
				if (!whirlwindcd)
				{
					_unit->CastSpell(_unit, LEOTHERAS_WHIRLWIND, true);
					_unit->GetAIInterface()->disable_melee = true;
					Unit *target = RandomTarget(true, true, 10000);
					if (target)
						_unit->GetAIInterface()->_CalcDestinationAndMove(target, 2);
					whirlwindcd = 16;
					whirlwinding = 1;
				}
			}
			else
			{
				whirlwinding++;
				Unit *target;
				switch (whirlwinding)
				{
				case 3:
				case 5:
				case 7:
				case 9:
				case 11:
					target = RandomTarget(true, true, 10000);
					if (target)
						_unit->GetAIInterface()->_CalcDestinationAndMove(target, 2);
					break;
				case 13:
					whirlwinding = 0;
					_unit->GetAIInterface()->disable_melee = false;
					_unit->GetAIInterface()->WipeHateList();
					break;
				}
			}
			phasecd--;
		}
	}
	void PhaseTwo()
	{
		if (!phasecd)
		{

			m_phase = 1;
			if (_unit->GetCurrentSpell())
			{
				_unit->GetCurrentSpell()->cancel();
			}
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 20514);
			_unit->GetAIInterface()->disable_melee = false;
			_unit->GetAIInterface()->WipeHateList();
			phasecd = 45;
		}
		else 
		{
			if (!innerdemonscd && !innerdemons)
			{
				if (GetPlayerCount() >= 6)
				{
					_unit->PlaySoundToSet(11305);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We all have our demons");
					_unit->CastSpell(_unit, LEOTHERAS_INSIDIOUSWHISPER, false);
					Player *target1 = (Player *) RandomTarget(false, true, 10000);
					Player *target2 = (Player *) RandomTarget(false, true, 10000);
					while(target2 == target1)
					{
						target2 = (Player *) RandomTarget(false, true, 10000);
					}
					Player *target3 = (Player *) RandomTarget(false, true, 10000);
					while (target3 == target1 || target3 == target2)
					{
						target3 = (Player *) RandomTarget(false, true, 10000);
					}
					Player *target4 = (Player *) RandomTarget(false, true, 10000);
					while (target4 == target1 || target4 == target2 || target4 == target3)
					{
						target4 = (Player *) RandomTarget(false, true, 10000);
					}
					Player *target5 =  (Player *) RandomTarget(false, true, 10000);
					while (target5 == target1 || target5 == target2 || target5 == target3 || target5 == target4)
					{
						target5 = (Player *) RandomTarget(false, true, 10000);
					}
					Creature *summon1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_INNERDEMON, target1->GetPositionX(), target1->GetPositionY(), target1->GetPositionZ(), target1->GetOrientation(), true, false, 0, 0);
					summon1->GetAIInterface()->SetSoulLinkedWith(target1);
					summon1->GetAIInterface()->AttackReaction( target1, 10000, 0);
					Creature *summon2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_INNERDEMON, target2->GetPositionX(), target2->GetPositionY(), target2->GetPositionZ(), target2->GetOrientation(), true, false, 0, 0);
					summon2->GetAIInterface()->SetSoulLinkedWith(target2);
					summon2->GetAIInterface()->AttackReaction( target2, 10000, 0);
					Creature *summon3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_INNERDEMON, target3->GetPositionX(), target3->GetPositionY(), target3->GetPositionZ(), target3->GetOrientation(), true, false, 0, 0);
					summon3->GetAIInterface()->SetSoulLinkedWith(target3);
					summon3->GetAIInterface()->AttackReaction( target3, 10000, 0);
					Creature *summon4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_INNERDEMON, target4->GetPositionX(), target4->GetPositionY(), target4->GetPositionZ(), target4->GetOrientation(), true, false, 0, 0);
					summon4->GetAIInterface()->SetSoulLinkedWith(target4);
					summon4->GetAIInterface()->AttackReaction( target4, 10000, 0);
					Creature *summon5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_INNERDEMON, target5->GetPositionX(), target5->GetPositionY(), target5->GetPositionZ(), target5->GetOrientation(), true, false, 0, 0);
					summon5->GetAIInterface()->SetSoulLinkedWith(target5);
					summon5->GetAIInterface()->AttackReaction( target5, 100000, 0);
				}
				innerdemons = 1;
			}
			else
			{
				if (!chaosblastcd)
				{
					if(_unit->GetCurrentSpell() == NULL)
					{
						Unit *target = _unit->GetAIInterface()->GetNextTarget();
						LocationVector locvec( target->GetPositionX() , target->GetPositionY() , target->GetPositionZ() );
						uint32 dist = FL2UINT(_unit->CalcDistance(locvec));
						if (dist < 40)
						{
							_unit->CastSpell(target, LEOTHERAS_CHAOSBLAST, false);
							chaosblastcd = 2;
						}
						else
						{
							_unit->GetAIInterface()->_CalcDestinationAndMove(target, 32);
						}
					}
				}
				else
				{
					chaosblastcd--;
				}
				innerdemonscd--;
			}
			phasecd--;
		}
	}
	void PhaseThree()
	{
		if (!whirlwinding)
		{
			whirlwindcd--;
			if (!whirlwindcd)
			{
				_unit->CastSpell(_unit, LEOTHERAS_WHIRLWIND, true);
				_unit->GetAIInterface()->disable_melee = true;
				Unit *target = RandomTarget(true, true, 10000);
				if (target)
					_unit->GetAIInterface()->_CalcDestinationAndMove(target, 2);
				whirlwindcd = 16;
				whirlwinding = 1;
			}
		}
		else
		{
			whirlwinding++;
			Unit *target;
			switch (whirlwinding)
			{
			case 3:
			case 5:
			case 7:
			case 9:
			case 11:
				target = RandomTarget(true, true, 10000);
				if (target)
					_unit->GetAIInterface()->_CalcDestinationAndMove(target, 2);
				break;
			case 13:
				whirlwinding = 0;
				_unit->GetAIInterface()->disable_melee = false;
				_unit->GetAIInterface()->WipeHateList();
				break;
			}
		}
	}

	void EventStart(Unit* mTarget)
	{
		if (!m_eventstarted)
		{
			m_eventstarted = true;
			channeler1->GetAIInterface()->AttackReaction(mTarget, 0, 0);
			channeler2->GetAIInterface()->AttackReaction(mTarget, 0, 0);
			channeler3->GetAIInterface()->AttackReaction(mTarget, 0, 0);
		}
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}
	int GetPlayerCount()
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return 0;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (temp->GetTypeId() == TYPEID_PLAYER)
			{
				targetTable.push_back(temp);
			}
		}
		if (!targetTable.size())
			return 0;

		return (int)targetTable.size();
	}

protected:
	int whirlwindcd;
	int whirlwinding;
	int phasecd;
	int chaosblastcd;
	int innerdemonscd;
	int innerdemons;
	int m_enrage;
	int enraged;

	bool m_eventstarted;
	int m_phase;
	Creature *shadow;

	Creature *channeler1;
	Creature *channeler2;
	Creature *channeler3;
};


// Shadow of Leotheras

#define CN_LEOTHERASSHADOW 21875
#define SHADOWOFLEOTHERAS_CHAOSBLAST 37674

class LEOTHERASSHADOWAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LEOTHERASSHADOWAI);

	LEOTHERASSHADOWAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		chaosblastcd = 2;
	}

	void OnCombatStart(Unit* mTarget)
	{
		_unit->PlaySoundToSet(11309);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "At last I am liberated. It has been too long since I have tasted true freedom!");
		_unit->GetAIInterface()->disable_melee = true;
		chaosblastcd = 0;
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		if (!chaosblastcd)
		{
			if(_unit->GetCurrentSpell() == NULL)
			{
				Unit *target = _unit->GetAIInterface()->GetNextTarget();
				LocationVector locvec( target->GetPositionX() , target->GetPositionY() , target->GetPositionZ() );
				uint32 dist = FL2UINT(_unit->CalcDistance(locvec));
				if (dist < 40)
				{
					_unit->CastSpell(target, SHADOWOFLEOTHERAS_CHAOSBLAST, false);
					chaosblastcd = 2;
				}
				else
				{
					_unit->GetAIInterface()->_CalcDestinationAndMove(_unit->GetAIInterface()->GetNextTarget(), 32);
				}
			}
		}
		else
		{
			chaosblastcd--;
		}
	}

protected:
	uint32 chaosblastcd;
};

// Greyheart Spellbinder
#define CN_SPELLBINDER 21806
#define CN_LEOTHERAS 21215

#define SPELLBINDER_MINDBLAST 37531


class GREYHEARTSPELLBINDERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GREYHEARTSPELLBINDERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	GREYHEARTSPELLBINDERAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(SPELLBINDER_MINDBLAST);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = false;
		spells[0].cooldown = 12;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		leotheras = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(380.700989f, -445.562988f, 29.525999f, CN_LEOTHERAS);
		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, leotheras->GetGUID());
		_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 30662);
	}
	void OnCombatStart(Unit* mTarget)
	{
		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
		_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
		CastTime();
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (leotheras)
		{
			CreatureAIScript *mob_script = leotheras->GetScript();
			((LEOTHERASAI*)mob_script)->EventStart(mTarget);
		}
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}
	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->GetAIInterface()->SetNextTarget(target);
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

protected:
	int nrspells;
	Creature *leotheras;
};

//Lady Vashj

#define CN_VASHJ 21212
#define CN_COILFANGSTRIDER 22056
#define CN_COILFANGELITE 22055
#define CN_ENCHANTEDELEMTAL 21958
#define CN_TAINTEDELEMTAL 22009
#define CN_TOXICSPOREBAT 22140

#define VASHJ_MULTISHOOT 38310
#define VASHJ_SHOOT 39079
#define VASHJ_FORKEDLIGHTNING 40088
#define VASHJ_ENTANGLE 38316
#define VASHJ_STATICCHARGE 38280
#define VASHJ_SHIELD 41373

class VASHJAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(VASHJAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	VASHJAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		nrspells = 1;
		for(int i=0; i<nrspells; i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(VASHJ_STATICCHARGE);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
	}

	void OnCombatStart(Unit* mTarget)
	{
		int val = RandomUInt(100)%4;
		switch (val)
		{
		case 0:
			//_unit->PlaySoundToSet(11532); wrong need to find the right one
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I did not wish to low myself by engaging your kind, but you leave me little coice... ");
			break;
		case 1:
			_unit->PlaySoundToSet(11533);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Victory to Lord Illidan! ");
			break;
		case 2:
			_unit->PlaySoundToSet(11534);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I spit on you, surface filth! ");
			break;
		case 3:
			_unit->PlaySoundToSet(11535);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Death to the outsiders! Victory to Lord Illidan! ");
			break;

		}

		_unit->DamageDoneModPCT[0] = 0;
		_unit->DamageDoneModPCT[1] = 0;
		_unit->DamageDoneModPCT[2] = 0;
		_unit->DamageDoneModPCT[3] = 0;
		_unit->DamageDoneModPCT[4] = 0;
		_unit->DamageDoneModPCT[5] = 0;
		_unit->DamageDoneModPCT[6] = 0;
		m_phase = 1;
		multishootcd = 5;
		entanglecd = 30 + RandomUInt(100)%15;

		RegisterAIUpdateEvent(1000);
		CastTime();
	}

	void OnCombatStop(Unit *mTarget)
	{
		if (_unit->isAlive())
		{
			_unit->RemoveAuraVisual(VASHJ_SHIELD, 1);
			_unit->SchoolImmunityList[0] = 0;
			_unit->SchoolImmunityList[1] = 0;
			_unit->SchoolImmunityList[2] = 0;
			_unit->SchoolImmunityList[3] = 0;
			_unit->SchoolImmunityList[4] = 0;
			_unit->SchoolImmunityList[5] = 0;
			_unit->SchoolImmunityList[6] = 0;
			_unit->DamageDoneModPCT[0] = 0;
			_unit->DamageDoneModPCT[1] = 0;
			_unit->DamageDoneModPCT[2] = 0;
			_unit->DamageDoneModPCT[3] = 0;
			_unit->DamageDoneModPCT[4] = 0;
			_unit->DamageDoneModPCT[5] = 0;
			_unit->DamageDoneModPCT[6] = 0;
			_unit->GetAIInterface()->disable_melee = false;
		}
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		_unit->PlaySoundToSet(11544);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lord Illidan, I... I am... sorry.");
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{
		int val = RandomUInt(100)%2;
		switch (val)
		{
		case 0:
			_unit->PlaySoundToSet(11541);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your time ends now!");
			break;
		case 1:
			_unit->PlaySoundToSet(11542);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have failed!");
			break;
		}
	}
	void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 70 && m_phase == 1)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
			_unit->PlaySoundToSet(11539);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The time is now! Leave none standing!");
			_unit->AddAuraVisual(VASHJ_SHIELD, 1, true);
			_unit->SchoolImmunityList[0] = 1;
			_unit->SchoolImmunityList[1] = 1;
			_unit->SchoolImmunityList[2] = 1;
			_unit->SchoolImmunityList[3] = 1;
			_unit->SchoolImmunityList[4] = 1;
			_unit->SchoolImmunityList[5] = 1;
			_unit->SchoolImmunityList[6] = 1;
			_unit->GetAIInterface()->MoveTo(_unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ(), 0.0f);
			_unit->GetAIInterface()->disable_melee = true;
			coilfangelitecd = 48;
			coilfangstridercd = 62;
			taintedelemtalcd = 53;
			enchantedelemtalcd = 4;
			forgedlightningcd = 5;
			m_phase = 2;
		}
		else if (_unit->GetHealthPct() <= 50 && m_phase == 2)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_MELEE);
			_unit->PlaySoundToSet(11540);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You may want to take cover.");
			_unit->RemoveAuraVisual(VASHJ_SHIELD, 1);
			_unit->SchoolImmunityList[0] = 0;
			_unit->SchoolImmunityList[1] = 0;
			_unit->SchoolImmunityList[2] = 0;
			_unit->SchoolImmunityList[3] = 0;
			_unit->SchoolImmunityList[4] = 0;
			_unit->SchoolImmunityList[5] = 0;
			_unit->SchoolImmunityList[6] = 0;
			_unit->GetAIInterface()->WipeHateList();
			_unit->GetAIInterface()->disable_melee = false;
			multishootcd = 5;
			shootcd = 1;
			entanglecd = 30 + RandomUInt(100)%15;
			toxicsporebatcd = 3;
			toxicsporebats = 25;
			Creature *summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TOXICSPOREBAT, -59.524456f, -898.862793f, 60.000000f, 4.697067f, true, false, 0, 0);
			m_phase = 3;
		}
		switch (m_phase)
		{
		case 1:
			PhaseOne();
			break;
		case 2:
			PhaseTwo();
			break;
		case 3:
			PhaseThree();
			break;
		}
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->GetAIInterface()->SetNextTarget(target);
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

	void PhaseOne()
	{
		if ((_unit->GetCurrentSpell() == NULL) && (RandomTarget(true, false, 100) == NULL))
		{
			if (multishootcd >= 15)
			{
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), VASHJ_MULTISHOOT, false);
				multishootcd = 0;
			}
			else
			{
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), VASHJ_SHOOT, false);
			}
		}
		else
		{
			if (!entanglecd)
			{
				int val = RandomUInt(100)%2;
				switch (val)
				{
				case 0:
					_unit->PlaySoundToSet(11536);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Straight to the heart!");
					break;
				case 1:
					_unit->PlaySoundToSet(11537);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Seek your mark!");
					break;
				}
				_unit->CastSpell(_unit, VASHJ_ENTANGLE, true);
				_unit->GetAIInterface()->_CalcDestinationAndMove(_unit->GetAIInterface()->GetNextTarget(), 20);
				entanglecd = 30 + RandomUInt(100)%15;
			}
			else
			{
				entanglecd--;
			}
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
		multishootcd++;
	}
	void PhaseTwo()
	{
		_unit->GetAIInterface()->AttackReaction(_unit->GetAIInterface()->GetMostHated(), 0, 0);
		if (!forgedlightningcd)
		{
			int spelldest = RandomUInt(100)%4;
			switch (spelldest)
			{
			case 0:
				_unit->CastSpellAoF( 29.738218f, -894.18377f, 41.545170f, dbcSpell.LookupEntry(VASHJ_FORKEDLIGHTNING), false);
				break;
			case 1:
				_unit->CastSpellAoF( 0.753696f, -923.398010f, 41.549282f, dbcSpell.LookupEntry(VASHJ_FORKEDLIGHTNING), false);
				break;
			case 2:
				_unit->CastSpellAoF( 29.772905f, -952.652039f, 41.545105f, dbcSpell.LookupEntry(VASHJ_FORKEDLIGHTNING), false);
				break;
			case 3:
				_unit->CastSpellAoF( 58.988274f, -923.393005f, 41.545250f, dbcSpell.LookupEntry(VASHJ_FORKEDLIGHTNING), false);
			}
			forgedlightningcd = 5;
		}
		else
		{
			forgedlightningcd--;
		}
		int val;
		if (taintedelemtalcd)
		{
			if (!enchantedelemtalcd)
			{
				val = RandomUInt(100)%9;
				Creature *summon;
				switch (val)
				{
				case 0:
					summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTEDELEMTAL, 95.416428f, -858.470093f, 20.874939f, 3.928170f, true, false, 0, 0);
					break;
				case 1:
					summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTEDELEMTAL, 54.330906f, -834.683411f, 21.043386f, 4.458308f, true, false, 0, 0);
					break;
				case 2:
					summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTEDELEMTAL, 6.822807f, -833.937500f, 20.874128f, 4.970778f, true, false, 0, 0);
					break;
				case 3:
					summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTEDELEMTAL, -59.524456f, -898.862793f, 20.744196f, 6.009459f, true, false, 0, 0);
					break;
				case 4:
					summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTEDELEMTAL, -36.159065f, -988.489197f, 20.725292f, 0.770832f, true, false, 0, 0);
					break;
				case 5:
					summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTEDELEMTAL, 4.9164569f, -1012.535767f, 20.787289f, 1.322573f, true, false, 0, 0);
					break;
				case 6:
					summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTEDELEMTAL, 52.985622f, -1012.738708f, 20.913624f, 1.835046f, true, false, 0, 0);
					break;
				case 7:
					summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTEDELEMTAL, 96.840096f, -987.219666f, 20.758579f, 2.343590f, true, false, 0, 0);
					break;
				case 8:
					summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENCHANTEDELEMTAL, 118.946030f, -948.197021f, 20.749378f, 2.861951f, true, false, 0, 0);
					break;
				}
				enchantedelemtalcd = 9;
			}
			else
			{
				enchantedelemtalcd--;
			}
			taintedelemtalcd--;
		}
		else
		{
			val = RandomUInt(100)%9;
			Creature *summon;
			switch (val)
			{
			case 0:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDELEMTAL, 95.416428f, -858.470093f, 20.874939f, 3.928170f, true, false, 0, 0);
				break;
			case 1:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDELEMTAL, 54.330906f, -834.683411f, 21.043386f, 4.458308f, true, false, 0, 0);
				break;
			case 2:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDELEMTAL, 6.822807f, -833.937500f, 20.874128f, 4.970778f, true, false, 0, 0);
				break;
			case 3:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDELEMTAL, -59.524456f, -898.862793f, 20.744196f, 6.009459f, true, false, 0, 0);
				break;
			case 4:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDELEMTAL, -36.159065f, -988.489197f, 20.725292f, 0.770832f, true, false, 0, 0);
				break;
			case 5:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDELEMTAL, 4.9164569f, -1012.535767f, 20.787289f, 1.322573f, true, false, 0, 0);
				break;
			case 6:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDELEMTAL, 52.985622f, -1012.738708f, 20.913624f, 1.835046f, true, false, 0, 0);
				break;
			case 7:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDELEMTAL, 96.840096f, -987.219666f, 20.758579f, 2.343590f, true, false, 0, 0);
				break;
			case 8:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TAINTEDELEMTAL, 118.946030f, -948.197021f, 20.749378f, 2.861951f, true, false, 0, 0);
				break;
			}
			taintedelemtalcd = 53;
			enchantedelemtalcd = 9;
		}
		if (!coilfangelitecd)
		{
			val = RandomUInt(100)%9;
			Creature *summon;
			switch (val)
			{
			case 0:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGELITE, 95.416428f, -858.470093f, 20.874939f, 3.928170f, true, false, 0, 0);
				break;
			case 1:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGELITE, 54.330906f, -834.683411f, 21.043386f, 4.458308f, true, false, 0, 0);
				break;
			case 2:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGELITE, 6.822807f, -833.937500f, 20.874128f, 4.970778f, true, false, 0, 0);
				break;
			case 3:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGELITE, -59.524456f, -898.862793f, 20.744196f, 6.009459f, true, false, 0, 0);
				break;
			case 4:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGELITE, -36.159065f, -988.489197f, 20.725292f, 0.770832f, true, false, 0, 0);
				break;
			case 5:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGELITE, 4.9164569f, -1012.535767f, 20.787289f, 1.322573f, true, false, 0, 0);
				break;
			case 6:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGELITE, 52.985622f, -1012.738708f, 20.913624f, 1.835046f, true, false, 0, 0);
				break;
			case 7:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGELITE, 96.840096f, -987.219666f, 20.758579f, 2.343590f, true, false, 0, 0);
				break;
			case 8:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGELITE, 118.946030f, -948.197021f, 20.749378f, 2.861951f, true, false, 0, 0);
				break;
			}
			coilfangelitecd = 48;
		}
		else
		{
			coilfangelitecd--;
		}
		if (!coilfangstridercd)
		{
			val = RandomUInt(100)%2;
			Creature *summon;
			switch (val)
			{
			case 0:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGSTRIDER, 29.615667f, -1017.765625f, 21.570890f, 1.557591f, true, false, 0, 0);
				break;
			case 1:
				summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COILFANGSTRIDER, 29.688040f, -827.500061f, 20.697882f, 4.703711f, true, false, 0, 0);
				break;
			}
			coilfangstridercd = 62;
		}
		else
		{
			coilfangstridercd--;
		}
	}
	void PhaseThree()
	{
		if ((_unit->GetCurrentSpell() == NULL) && (RandomTarget(true, false, 100) == NULL))
		{
			if (multishootcd >= 15)
			{
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), VASHJ_MULTISHOOT, false);
				multishootcd = 0;
			}
			else
			{
				if (!shootcd)
				{
					_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), VASHJ_SHOOT, false);
					shootcd = 1;
				}
				else
				{
					shootcd--;
				}
			}
		}
		else
		{
			if (!entanglecd)
			{
				int val = RandomUInt(100)%2;
				switch (val)
				{
				case 0:
					_unit->PlaySoundToSet(11536);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Straight to the heart!");
					break;
				case 1:
					_unit->PlaySoundToSet(11537);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Seek your mark!");
					break;
				}
				_unit->CastSpell(_unit, VASHJ_ENTANGLE, true);
				_unit->GetAIInterface()->_CalcDestinationAndMove(_unit->GetAIInterface()->GetNextTarget(), 20);
				entanglecd = 30 + RandomUInt(100)%15;
			}
			else
			{
				entanglecd--;
			}
		}
		if (!toxicsporebatcd)
		{
			Creature *summon = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_TOXICSPOREBAT, -59.524456f, -898.862793f, 60.000000f, 4.697067f, true, false, 0, 0);
			toxicsporebatcd = 5 + toxicsporebats;
			if (toxicsporebats)
			{
				toxicsporebats--;
			}
		}
		else
		{
			toxicsporebatcd--;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
		multishootcd++;
	}

protected:
	int m_phase;
	int nrspells;
	int multishootcd;
	int entanglecd;
	int shootcd;
	int forgedlightningcd;

	int coilfangelitecd;
	int coilfangstridercd;
	int taintedelemtalcd;
	int enchantedelemtalcd;
	int toxicsporebatcd;
	int toxicsporebats;
};

//Coilfang Strider

#define CN_COILFANGSTRIDER 22056

#define COILFANGSTRIDER_PANIC 38258
#define COILFANGSTRIDER_MINDBLAST 38259

static Coords coilfangstrider[] = 
{
	{ 0, 0, 0, 0 },
	{ 29.605434f, -983.263000f, 41.043144f, 1.579787f },
	{ 29.681782f, -863.706665f, 41.097023f, 4.703711f }
};

class COILFANGSTRIDERAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGSTRIDERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	COILFANGSTRIDERAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, RUN));

		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);

		if ((_unit->GetPositionX() == 29.615667f) && (_unit->GetPositionY() == -1017.765625f) && (_unit->GetPositionZ() == 21.570890f))
		{
			_unit->GetAIInterface()->setWaypointToMove(1);
		}
		else if ((_unit->GetPositionX() == 29.688040f) && (_unit->GetPositionY() == -827.500061f) && (_unit->GetPositionZ() == 20.697882f))
		{
			_unit->GetAIInterface()->setWaypointToMove(2);
		}

		spells[0].info = dbcSpell.LookupEntry(COILFANGSTRIDER_MINDBLAST);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		m_fear = 2;
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
		CastTime();
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(5000, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		if (!m_fear)
		{
			_unit->CastSpell(_unit, COILFANGSTRIDER_PANIC, true);
			m_fear = 2;
		}
		else
		{
			m_fear--;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = coilfangstrider[id].x;
		wp->y = coilfangstrider[id].y;
		wp->z = coilfangstrider[id].z;
		wp->o = coilfangstrider[id].o;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}
protected:
	int m_fear;
	int nrspells;
};

//Coilfang Elite

#define CN_COILFANGELITE 22055

#define COILFANGELITE_CLEAVE 30619

static Coords generalmove[] = 
{
	{ 0, 0, 0, 0 },
	{ 71.418457f, -883.553467f, 41.097008f, 3.928170f },
	{ 45.075714f, -867.637878f, 41.042625f, 4.458308f },
	{ 14.487659f, -867.653992f, 41.063335f, 4.970778f },
	{ -25.774921f, -908.236877f, 41.097076f, 6.009459f },
	{ -11.256523f, -963.961304f, 41.097065f, 0.770832f },
	{ 14.365232f, -979.019287f, 41.097065f, 1.322573f },
	{ 44.090549f, -979.027344f, 41.097080f, 1.835046f },
	{ 70.239227f, -964.545105f, 41.097076f, 2.343590f },
	{ 85.456764f, -938.862183f, 41.062885f, 2.861951f }
};

class COILFANGELITEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COILFANGELITEAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	COILFANGELITEAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(9, 0, RUN));

		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);

		if ((_unit->GetPositionX() == 95.416428f) && (_unit->GetPositionY() == -858.470093f) && (_unit->GetPositionZ() == 20.874939f))
		{
			_unit->GetAIInterface()->setWaypointToMove(1);
		}
		else if ((_unit->GetPositionX() == 54.330906f) && (_unit->GetPositionY() == -834.683411f) && (_unit->GetPositionZ() == 21.043386f))
		{
			_unit->GetAIInterface()->setWaypointToMove(2);
		}
		else if ((_unit->GetPositionX() == 6.822807f) && (_unit->GetPositionY() == -833.937500f) && (_unit->GetPositionZ() == 20.874128f))
		{
			_unit->GetAIInterface()->setWaypointToMove(3);
		}
		else if ((_unit->GetPositionX() == -59.524456f) && (_unit->GetPositionY() == -898.862793f) && (_unit->GetPositionZ() == 20.744196f))
		{
			_unit->GetAIInterface()->setWaypointToMove(4);
		}
		else if ((_unit->GetPositionX() == -36.159065f) && (_unit->GetPositionY() == -988.489197f) && (_unit->GetPositionZ() == 20.725292f))
		{
			_unit->GetAIInterface()->setWaypointToMove(5);
		}
		else if ((_unit->GetPositionX() == 4.9164569f) && (_unit->GetPositionY() == -1012.535767f) && (_unit->GetPositionZ() == 20.787289f))
		{
			_unit->GetAIInterface()->setWaypointToMove(6);
		}
		else if ((_unit->GetPositionX() == 52.985622f) && (_unit->GetPositionY() == -1012.738708f) && (_unit->GetPositionZ() == 20.913624f))
		{
			_unit->GetAIInterface()->setWaypointToMove(7);
		}
		else if ((_unit->GetPositionX() == 96.840096f) && (_unit->GetPositionY() == -987.219666f) && (_unit->GetPositionZ() == 20.758579f))
		{
			_unit->GetAIInterface()->setWaypointToMove(8);
		}
		else if ((_unit->GetPositionX() == 118.946030f) && (_unit->GetPositionY() == -948.197021f) && (_unit->GetPositionZ() == 20.749378f))
		{
			_unit->GetAIInterface()->setWaypointToMove(9);
		}

		nrspells = 1;
		for(int i=0; i<nrspells; i++)
		{
			m_spellcheck[i] = false;
		}
		spells[0].info = dbcSpell.LookupEntry(COILFANGELITE_CLEAVE);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
		CastTime();
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(5000, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = generalmove[id].x;
		wp->y = generalmove[id].y;
		wp->z = generalmove[id].z;
		wp->o = generalmove[id].o;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}
protected:
	int nrspells;
};

//Enchanted Elemental

#define CN_ENCHANTEDELEMTAL 21958
#define CN_VASHJ 21212

static Coords enchantedmove[] = 
{
	{ 0, 0, 0, 0 },
	{ 71.418457f, -883.553467f, 41.097008f, 3.928170f },
	{ 45.075714f, -867.637878f, 41.042625f, 4.458308f },
	{ 14.487659f, -867.653992f, 41.063335f, 4.970778f },
	{ -25.774921f, -908.236877f, 41.097076f, 6.009459f },
	{ -11.256523f, -963.961304f, 41.097065f, 0.770832f },
	{ 14.365232f, -979.019287f, 41.097065f, 1.322573f },
	{ 44.090549f, -979.027344f, 41.097080f, 1.835046f },
	{ 70.239227f, -964.545105f, 41.097076f, 2.343590f },
	{ 85.456764f, -938.862183f, 41.062885f, 2.861951f },
	{ 29.798161f, -923.358276f, 42.900517f, 0.000000f }
};

class ENCHANTEDELEMENTALAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ENCHANTEDELEMENTALAI);

	ENCHANTEDELEMENTALAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(9, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(10, 0, WALK));

		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);

		if ((_unit->GetPositionX() == 95.416428f) && (_unit->GetPositionY() == -858.470093f) && (_unit->GetPositionZ() == 20.874939f))
		{
			_unit->GetAIInterface()->setWaypointToMove(1);
		}
		else if ((_unit->GetPositionX() == 54.330906f) && (_unit->GetPositionY() == -834.683411f) && (_unit->GetPositionZ() == 21.043386f))
		{
			_unit->GetAIInterface()->setWaypointToMove(2);
		}
		else if ((_unit->GetPositionX() == 6.822807f) && (_unit->GetPositionY() == -833.937500f) && (_unit->GetPositionZ() == 20.874128f))
		{
			_unit->GetAIInterface()->setWaypointToMove(3);
		}
		else if ((_unit->GetPositionX() == -59.524456f) && (_unit->GetPositionY() == -898.862793f) && (_unit->GetPositionZ() == 20.744196f))
		{
			_unit->GetAIInterface()->setWaypointToMove(4);
		}
		else if ((_unit->GetPositionX() == -36.159065f) && (_unit->GetPositionY() == -988.489197f) && (_unit->GetPositionZ() == 20.725292f))
		{
			_unit->GetAIInterface()->setWaypointToMove(5);
		}
		else if ((_unit->GetPositionX() == 4.9164569f) && (_unit->GetPositionY() == -1012.535767f) && (_unit->GetPositionZ() == 20.787289f))
		{
			_unit->GetAIInterface()->setWaypointToMove(6);
		}
		else if ((_unit->GetPositionX() == 52.985622f) && (_unit->GetPositionY() == -1012.738708f) && (_unit->GetPositionZ() == 20.913624f))
		{
			_unit->GetAIInterface()->setWaypointToMove(7);
		}
		else if ((_unit->GetPositionX() == 96.840096f) && (_unit->GetPositionY() == -987.219666f) && (_unit->GetPositionZ() == 20.758579f))
		{
			_unit->GetAIInterface()->setWaypointToMove(8);
		}
		else if ((_unit->GetPositionX() == 118.946030f) && (_unit->GetPositionY() == -948.197021f) && (_unit->GetPositionZ() == 20.749378f))
		{
			_unit->GetAIInterface()->setWaypointToMove(9);
		}

		vashj = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(29.798161f, -923.358276f, 42.900517f, CN_VASHJ);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(5000, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch (iWaypointId)
		{
		case 10:
			vashj->DamageDoneModPCT[0] += 0.05f;
			vashj->DamageDoneModPCT[1] += 0.05f;
			vashj->DamageDoneModPCT[2] += 0.05f;
			vashj->DamageDoneModPCT[3] += 0.05f;
			vashj->DamageDoneModPCT[4] += 0.05f;
			vashj->DamageDoneModPCT[5] += 0.05f;
			vashj->DamageDoneModPCT[6] += 0.05f;
			_unit->Despawn(100, 0);
			break;
		default:
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(10);
			break;
		}
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = enchantedmove[id].x;
		wp->y = enchantedmove[id].y;
		wp->z = enchantedmove[id].z;
		wp->o = enchantedmove[id].o;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}
protected:
	Creature *vashj;
};

//Tainted Elemental

#define CN_TAINTEDELEMTAL 22009

class TAINTEDELEMTALAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TAINTEDELEMTALAI);

	TAINTEDELEMTALAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		vashj = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(29.798161f, -923.358276f, 42.900517f, CN_VASHJ);
		_unit->Despawn(15000, 0);
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		if (vashj && vashj->isAlive())
			vashj->SetUInt32Value(UNIT_FIELD_HEALTH, vashj->GetUInt32Value(UNIT_FIELD_HEALTH) - (vashj->GetUInt32Value(UNIT_FIELD_MAXHEALTH) / 20));
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
	}
protected:
	Creature *vashj;
};

//Toxic Spore Bat - WHY THEY DONT REALLY FLY?!?!

#define CN_TOXICSPOREBAT 22140

static Coords fly[] = 
{
	{ 0, 0, 0, 0 },
	{ 29.769f, -866.190f, 60, 0 },
	{ 1.135f, -874.345f, 60, 0 },
	{ -19.719f, -894.950f, 60, 0 },
	{ -27.4222f, -923.572f, 60, 0 },
	{ -19.739f, -951.907f, 60, 0 },
	{ 1.059f, -973.314f, 60, 0 },
	{ 30.071f, -980.424f, 60, 0 },
	{ 58.665f, -973.410f, 60, 0 },
	{ 79.353f, -952.011f, 60, 0 },
	{ 87.552f, -923.175f, 60, 0 },
	{ 79.068f, -894.570f, 60, 0 },
	{ 58.503f, -873.295f, 60, 0 }
};

class TOXICSPOREBATAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TOXICSPOREBATAI);

	TOXICSPOREBATAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{

		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(9, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(10, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(11, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(12, 0, FLY));

		_unit->GetAIInterface()->m_moveFly = true;
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		_unit->GetAIInterface()->setWaypointToMove(1);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		sporecd = 30;
		RegisterAIUpdateEvent(1000);
	}

	void OnCombatStart(Unit* mTarget)
	{
	}

	void OnCombatStop(Unit *mTarget)
	{
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn(100, 0);
	}

	void AIUpdate()
	{
		if (!sporecd)
		{
			Unit *target = RandomTarget();
			if (target)
				_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), dbcSpell.LookupEntry(38575), true); 
			else
				_unit->Despawn(100, 0);
			sporecd = 15;
		}
		else
		{
			sporecd--;
		}
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch(iWaypointId)
		{
		case 12:
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
			break;
		default:
			iWaypointId++;
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(iWaypointId);
			break;
		}
	}

	Unit *RandomTarget()
	{
		if (_unit->GetInRangePlayersCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap::iterator itr;
		for (set<Player*>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); itr++)
		{
			Unit *temp = (Unit*)(*itr);
			if (temp->isAlive())
				targetTable.push_back(temp);
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = fly[id].x;
		wp->y = fly[id].y;
		wp->z = fly[id].z;
		wp->o = fly[id].o;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}

protected:
	int nrspells;
	int sporecd;
};

//Underbog Colossus

#define CN_UNDERBOGCOLOSSUS 21251
#define CN_COLOSSUSLURKER 22347
#define CN_COLOSSUSRAGER 22352

//Type A:
#define RAMPANTINFECTION 39042
#define SPOREQUAKE 38976
#define SPOREQUAKEKNOCKDOWN 39002


//Type B:
#define PARASITES 39044
#define ACIDGEYSER 38971

//Type C:
#define FRENZY 39031
#define ATROPHICBLOW 39015

//On Death:
#define TOXICPOOL 38718
#define REFRESHINGMIST 38730

//Vulnerabiltiy
#define NATUREVULNERABILITY 38717
#define FROSTVULNERABILITY 38714
#define FIREVULNERABILITY 38715

class UNDERBOGCOLOSSUSAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(UNDERBOGCOLOSSUSAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

	UNDERBOGCOLOSSUSAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		int val = RandomUInt(100)%3;
		switch (val)
		{
		case 0:
			m_type = 1;
			nrspells = 1;
			specialcd = 12;

			spells[0].info = dbcSpell.LookupEntry(RAMPANTINFECTION);
			spells[0].targettype = TARGET_RANDOM_SINGLE;
			spells[0].instant = true;
			spells[0].cooldown = 15;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;
			break;
		case 1:
			m_type = 2;
			nrspells = 1;
			specialcd = 8;

			spells[0].info = dbcSpell.LookupEntry(ACIDGEYSER);
			spells[0].targettype = TARGET_RANDOM_SINGLE;
			spells[0].instant = false;
			spells[0].cooldown = 36;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 8000;
			break;
		case 2:
			nrspells = 2;

			spells[0].info = dbcSpell.LookupEntry(FRENZY);
			spells[0].targettype = TARGET_SELF;
			spells[0].instant = true;
			spells[0].cooldown = 20;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(ATROPHICBLOW);
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].cooldown = 16;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 1000;
			break;
		}
		val = RandomUInt(100)%3;
		switch (val)
		{
		case 0:
			_unit->CastSpell(_unit, FIREVULNERABILITY, true);
			break;
		case 1:
			_unit->CastSpell(_unit, FROSTVULNERABILITY, true);
			break;
		case 2:
			_unit->CastSpell(_unit, NATUREVULNERABILITY, true);
			break;
		}
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
		CastTime();
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		int amount;
		Unit *target;
		int val = RandomUInt(100)%5;
		switch (val)
		{
		case 0:
			_unit->CastSpellAoF(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), dbcSpell.LookupEntry(TOXICPOOL), true);
			break;
		case 1:
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COLOSSUSLURKER, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COLOSSUSLURKER, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			break;
		case 2:
			target = RandomTarget(true, true, 10000);
			if (target)
				_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), dbcSpell.LookupEntry(REFRESHINGMIST), true);
			break;
		case 3:
			amount = 10 + RandomUInt(100)%6;
			for (int x = 0; x < amount;x++)
			{
				_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_COLOSSUSRAGER, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			}
			break;
		case 4:
			// nothing :)
			break;
		}
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		switch (m_type)
		{
		case 1:
			if (!quaking)
			{
				if (!specialcd)
				{
					//missing emotestate, cant find it
					_unit->CastSpell(_unit, SPOREQUAKE, true);
					_unit->setAttackTimer(8000, false);
					quaking = 1;
					specialcd = 40;
				}
				else
					specialcd--;
			}
			else
			{
				quaking++;
				switch (quaking)
				{
				case 2:
				case 4:
				case 6:
					sporequakeknockdown();
					break;
				case 9:
					quaking = 0;
					break;
				}
			}
			break;
		case 2:
			if (!specialcd)
			{
				Unit *target = RandomTarget(false, true, 2500);
				if (target)
				{
					_unit->CastSpell(target, PARASITES, true);
				}
				specialcd = 15;
			}
			else
				specialcd--;
			break;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}
	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->GetAIInterface()->SetNextTarget(target);
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}
	void sporequakeknockdown()
	{
		if (_unit->GetAIInterface()->getAITargetsCount() > 0)
		{
			TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
			TargetMap::iterator itr;
			for (itr = targets->begin(); itr != targets->end();itr++)
			{
				if (_unit->GetDistance2dSq(itr->first) <= 900)
				{
					itr->first->CastSpell(itr->first, SPOREQUAKEKNOCKDOWN, true);
				}
			}
		}
	}

protected:
	int m_type;
	int nrspells;
	int specialcd;
	int quaking;
};

//Greyheart Nether-Mage

#define CN_GREYHEARTNETHERMAGE 21230

//Frost:
#define CONEOFCOLD 37265
#define FROSTBOLTVOLLEY 38837

//Fire:
#define RAINOFFIRE 39024
#define FIREBALLVOLLEY 38836

//Arcane:
#define ARCANELIGHTNING 38634
#define ARCANEVOLLEY 38835

#define BLINK 36937

class GREYHEARTNETHERMAGEAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GREYHEARTNETHERMAGEAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

	GREYHEARTNETHERMAGEAI(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		int val = RandomUInt(100)%3;
		switch (val)
		{
		case 0:
			m_type = 1;
			nrspells = 2;

			spells[0].info = dbcSpell.LookupEntry(CONEOFCOLD);
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].cooldown = 18;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(FROSTBOLTVOLLEY);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 12;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 1000;
			break;
		case 1:
			m_type = 2;
			nrspells = 2;

			spells[0].info = dbcSpell.LookupEntry(RAINOFFIRE);
			spells[0].targettype = TARGET_RANDOM_DESTINATION;
			spells[0].instant = true;
			spells[0].cooldown = 18;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(FIREBALLVOLLEY);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 12;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 1000;
			break;
		case 2:
			nrspells = 2;

			spells[0].info = dbcSpell.LookupEntry(ARCANELIGHTNING);
			spells[0].targettype = TARGET_RANDOM_SINGLE;
			spells[0].instant = true;
			spells[0].cooldown = 18;
			spells[0].perctrigger = 10.0f;
			spells[0].attackstoptimer = 1000;

			spells[1].info = dbcSpell.LookupEntry(ARCANEVOLLEY);
			spells[1].targettype = TARGET_VARIOUS;
			spells[1].instant = true;
			spells[1].cooldown = 12;
			spells[1].perctrigger = 10.0f;
			spells[1].attackstoptimer = 1000;
			break;
		}
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}
		specialcd = 10 + RandomUInt(100)%6;
	}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
		CastTime();
	}

	void OnCombatStop(Unit *mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mKiller)
	{
		RemoveAIUpdateEvent();
	}

	void OnTargetDied(Unit* mTarget)
	{ 
	}
	void AIUpdate()
	{
		if (!specialcd)
		{
			Unit *target = RandomTarget(false, true, 2500);
			if (target)
			{
				_unit->CastSpell(_unit, BLINK, true);
				_unit->SetPosition(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), false);
			}
			specialcd = 10 + RandomUInt(100)%6;
		}
		else
		{
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
			specialcd--;
		}
	}
	void SpellCast(float val)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			float comulativeperc = 0;
			Unit *target = NULL;
			for(int i=0; i<nrspells; i++)
			{
				if(!spells[i].perctrigger) continue;

				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_SELF:
					case TARGET_VARIOUS:
						_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
					case TARGET_ATTACKING:
						_unit->CastSpell(target, spells[i].info, spells[i].instant);
						break;
					case TARGET_DESTINATION:
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					case TARGET_RANDOM_SINGLE:
						target = RandomTarget(false, true, spells[i].info->base_range_or_radius_sqr);
						if (target)
						{
							_unit->GetAIInterface()->SetNextTarget(target);
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						}
						break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

	void CastTime()
	{
		for(int i=0; i<nrspells; i++)
			spells[i].casttime = spells[i].cooldown;
	}

	Unit *RandomTarget(bool tank,bool onlyplayer, float dist)
	{
		if (_unit->GetAIInterface()->getAITargetsCount() == 0)
			return NULL;

		std::vector<Unit*> targetTable;
		TargetMap *targets = _unit->GetAIInterface()->GetAITargets();
		TargetMap::iterator itr;
		for (itr = targets->begin(); itr != targets->end(); itr++)
		{
			Unit *temp = itr->first;
			if (_unit->GetDistance2dSq(temp) <= dist)
			{
				if (((!tank && temp != _unit->GetAIInterface()->GetNextTarget()) || tank) && (!onlyplayer || (onlyplayer && temp->GetTypeId() == TYPEID_PLAYER)))
				{
					targetTable.push_back(temp);
				}
			}
		}
		if (!targetTable.size())
			return NULL;

		uint32 randt = RandomUInt(100)%targetTable.size();
		Unit * randomtarget = targetTable[randt];
		return randomtarget;
	}

protected:
	int m_type;
	int nrspells;
	int specialcd;
};

void SetupSerpentshrineCavern(ScriptMgr * mgr)
{
	//Hydross the Unstable
	mgr->register_creature_script(CN_HYDROSS, &HYDROSSAI::Create);
	mgr->register_creature_script(CN_PURESPAWN, &HYDROSSPURESPAWNAI::Create);
	mgr->register_creature_script(CN_TAINTEDSPAWN, &HYDROSSTAINTEDSPAWNAI::Create);

	//Lurker Below
	mgr->register_creature_script(CN_LURKERBELOW, &LURKERBELOWAI::Create);
	mgr->register_creature_script(CN_COILFANGAMBUSHER, &COILFANGAMBUSHERAI::Create);
	mgr->register_creature_script(CN_COILFANGGUARDIAN, &COILFANGGUARDIANAI::Create);

	//Morogrim Tidewalker
	mgr->register_creature_script(CN_MOROGRIM, &MOROGRIMAI::Create);
	mgr->register_creature_script(CN_WATERGLOBULE, &WATERGLOBULEAI::Create);
	mgr->register_creature_script(CN_MURLOC, &MOROGRIMMURLOCAI::Create);

	//Fathom Lord Karathress
	mgr->register_creature_script(CN_KARATHRESS, &KARATHRESSAI::Create);
	mgr->register_creature_script(CN_FATHOMGUARDSHARKKIS, &SHARKKISAI::Create);
	mgr->register_creature_script(CN_FATHOMGUARDCARIBDIS, &CARIBDISAI::Create);
	mgr->register_creature_script(CN_FATHOMGUARDTIDALVESS, &TIDALVESSAI::Create);
	mgr->register_creature_script(CN_FATHOMLURKER, &SHARKISSPETSAI::Create);
	mgr->register_creature_script(CN_FATHOMSPOREBAT, &SHARKISSPETSAI::Create);
	mgr->register_creature_script(CN_SPITFIRE, &SPITFIRETOTEMAI::Create);
	mgr->register_creature_script(CN_EARTHBIND, &EARTHBINDTOTEMAI::Create);

	//Leotheras the Blind
	mgr->register_creature_script(CN_LEOTHERAS, &LEOTHERASAI::Create);
	mgr->register_creature_script(CN_LEOTHERASSHADOW, &LEOTHERASSHADOWAI::Create);
	mgr->register_creature_script(CN_SPELLBINDER, &GREYHEARTSPELLBINDERAI::Create);

	//Lady Vashj
	mgr->register_creature_script(CN_VASHJ, &VASHJAI::Create);
	mgr->register_creature_script(CN_COILFANGSTRIDER, &COILFANGSTRIDERAI::Create);
	mgr->register_creature_script(CN_COILFANGELITE, &COILFANGELITEAI::Create);
	mgr->register_creature_script(CN_ENCHANTEDELEMTAL, &ENCHANTEDELEMENTALAI::Create);
	mgr->register_creature_script(CN_TAINTEDELEMTAL, &TAINTEDELEMTALAI::Create);
	mgr->register_creature_script(CN_TOXICSPOREBAT, &TOXICSPOREBATAI::Create);

	//Trash Mobs
	mgr->register_creature_script(CN_UNDERBOGCOLOSSUS, &UNDERBOGCOLOSSUSAI::Create);
	mgr->register_creature_script(CN_GREYHEARTNETHERMAGE, &GREYHEARTNETHERMAGEAI::Create);
}