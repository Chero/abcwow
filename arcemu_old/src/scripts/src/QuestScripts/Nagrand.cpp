/*
 * Modifyed on 20 July 2008 by Angelis, from SUN++ Community.
 * Copyright (C) 2008 Sun++ Team <http://sunplusplus.info/>
 *
 * --------------------------------------------------------------------
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"
#include "EAS/EasyFunctions.h"

// Giselda The Crone Quest
class GiseldaTheCroneQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GiseldaTheCroneQAI);
    GiseldaTheCroneQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer())
		{
			if (((Player*)mKiller)->GetTeam() == 0)
			{
				QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(9936);
				if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
				{
					uint32 newcount = en->GetMobCount(1) + 1;
					en->SetMobCount(1, newcount);
					en->SendUpdateAddKill(1);
					en->UpdatePlayerFields();
					return;
				}
			}
			else
			{
				QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(9935);
				if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
				{
					uint32 newcount = en->GetMobCount(1) + 1;
					en->SetMobCount(1, newcount);
					en->SendUpdateAddKill(1);
					en->UpdatePlayerFields();
					return;
				}
			}
		}
	}
};

//*********************************************************************************************
//							   The Ring of Blood
//*********************************************************************************************


class Quest_The_Ring_of_Blood_The_Final_Challenge : public QuestScript
{
public:
		void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
		{
			if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
				return;

			Creature * mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

			if(mogor)
			{
				mogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Prepare yourselves!");

				Unit * Qgiver = NULL;
				Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

				if(Qgiver)
				{
					char msg[256];
					snprintf((char*)msg, 256, "Mogor has challenged you. You have to accept! Get in the right of blood if you are ready to fight.", mTarget->GetName());
					Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
					string msg2 = "For the first time in the Ring of Bloods history. Mogor has chosen to exercise his right of the battle! On this wartorn ground, ";
					msg2 += mTarget->GetName();
					msg2 += "  will face Mogor, hero of the Warmaul!";
					sEventMgr.AddEvent(static_cast< Unit* >(Qgiver), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, msg2.c_str(), EVENT_UNIT_CHAT_MSG, 32000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

				}

				mogor->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				mogor->GetAIInterface()->SetAllowedToEnterCombat(true);
				mogor->GetAIInterface()->MoveTo(-704.669f, 7871.08f, 45.0387f, 1.59531f);
				mogor->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
			}
		}

		void OnQuestComplete(Player * mTarget, QuestLogEntry * qLogEntry)
		{
			if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
				return;

			Creature * mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

			if(mogor)
			{
				mogor->Despawn(1000,0);
			}

			mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18069, -712.443115f, 7932.182129f, 59.430191f, 4.515952f, true, false, 0, 0);
		}
};

class Quest_The_Ring_of_Blood_The_Warmaul_Champion : public QuestScript
{
public:
		void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
		{
			if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
				return;

			Unit * Qgiver = NULL;
			Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

			if(Qgiver)
			{
				char msg[256];
				snprintf((char*)msg, 256, "Get in the Ring of Blood, %s . The fight is about to start!", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				string msg2 = "They had to ship the champion in from the Blade's Edge gladiator pits. He was training on mountain giants - three at a time.";
				//char msg2[256];
				//snprintf((char*)msg2, 256, "They had to ship the champion in from the Blade's Edge gladiator pits. He was training on mountain giants - three at a time.", mTarget->GetName());
				sEventMgr.AddEvent(static_cast< Unit* >(Qgiver), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, msg2.c_str(), EVENT_UNIT_CHAT_MSG, 4000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18402, -704.669f, 7871.08f, 45.0387f, 1.59531f, true, false, 0, 0);
			}
		}

		void OnQuestComplete(Player * mTarget, QuestLogEntry * qLogEntry)
		{
			if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
				return;

					Unit * mogor = NULL;
					mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

			if(mogor)
			{
				char msg[256];
				snprintf((char*)msg, 256, "WUT!? UNPOSSIBLE!! You fight Mogor now! Mogor destroy!", mTarget->GetName());
				mogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
			}

			Creature * warmaulchamp = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18402);
			if(warmaulchamp)
			{
				warmaulchamp->Despawn(1000,0);
			}
	}

};

class Quest_The_Ring_of_Blood_Skragath : public QuestScript
{
public:
		void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
		{
			if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
				return;

			Unit * Qgiver = NULL;
			Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

			if(Qgiver)
			{
				char msg[256];
				snprintf((char*)msg, 256, "Get in the Ring of Blood, %s . The fight is about to start!", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				string msg2 = "From the parts unknown: Ska'gath! Can ";
				msg2 += mTarget->GetName();
				msg2 += " possibly survive the onslaught of void energies?";
				//char msg2[256];
				//snprintf((char*)msg2, 256, "From the parts unknown: Ska'gath! Can %s possibly survive the onslaught of void energies?", mTarget->GetName());
				sEventMgr.AddEvent(static_cast< Unit* >(Qgiver), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, msg2.c_str(), EVENT_UNIT_CHAT_MSG, 4000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18401, -704.669f, 7871.08f, 45.0387f, 1.59531f, true, false, 0, 0);
			}
		}
		void OnQuestComplete(Player * mTarget, QuestLogEntry * qLogEntry)
		{
			if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
				return;
			Unit * mogor = NULL;
			mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

				 if(mogor)
				 {
				   char msg[256];
				   snprintf((char*)msg, 256, "Mogor not impressed! Skra'gat wuz made of da air and shadow! Soft like da squishy orcies!", mTarget->GetName());
				   mogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				 }

				 Creature * skragath = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18401);
			if(skragath)
			{
			skragath->Despawn(1000,0);
			}
	  }
};

class Quest_The_Ring_of_Blood_Rokdar_the_Sundered_Lord : public QuestScript
{
public:
	  void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
	  {
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;

			Unit * Qgiver = NULL;
				Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

				if(Qgiver)
				{
				char msg[256];
				snprintf((char*)msg, 256, "Get in the Ring of Blood, %s . The fight is about to start!", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				string msg2 = "Hailing from the mountains of Blade's Edge comes Rokdar the Sundered Lord! ";
				msg2 += mTarget->GetName();
				msg2 += " is in for the fight of his life.";
				sEventMgr.AddEvent(static_cast< Unit* >(Qgiver), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, msg2.c_str(), EVENT_UNIT_CHAT_MSG, 4000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

				 mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18400, -704.669f, 7871.08f, 45.0387f, 1.59531f, true, false, 0, 0);
				}
	  }
	  void OnQuestComplete(Player * mTarget, QuestLogEntry * qLogEntry)
		{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
				 Unit * mogor = NULL;
				 mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

				 if(mogor)
				 {
				   char msg[256];
				   snprintf((char*)msg, 256, "...", mTarget->GetName());
				   mogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				 }

				 Creature * rokdar = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18400);
			if(rokdar)
			{
			rokdar->Despawn(1000,0);
			}
   }
};

class Quest_The_Ring_of_Blood_The_Blue_Brothers : public QuestScript
{
public:
	  void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
	  {
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		 Creature * brokentoe = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18398);
			if(brokentoe)
			{
			   brokentoe->Despawn(1000,0);
			}

			Unit * Qgiver = NULL;
				Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

				if(Qgiver)
				{
				char msg[256];
				snprintf((char*)msg, 256, "Get in the Ring of Blood, %s . The fight is about to start!", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				string msg2 = "The battle is about to begin! The unmerciful Murkblood twins versus ";
				msg2 += mTarget->GetName();
				sEventMgr.AddEvent(static_cast< Unit* >(Qgiver), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, msg2.c_str(), EVENT_UNIT_CHAT_MSG, 4000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18399, -704.669f, 7871.08f, 45.0387f, 1.59531f, true, false, 0, 0);
				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18399, -708.076f, 7870.41f, 44.8457f, 1.59531f, true, false, 0, 0);
				}
	  }
	  void OnQuestComplete(Player * mTarget, QuestLogEntry * qLogEntry)
		{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
			  Unit * mogor = NULL;
			  mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

				 if(mogor)
				 {
				   char msg[256];
				   snprintf((char*)msg, 256, "...", mTarget->GetName());
				   mogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				 }

				 Creature * brother1 = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18399);
			if(brother1)
			{
			   brother1->Despawn(1000,0);
			}
			Creature * brother2 = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18399);
	  if(brother2)
	  {
		  brother2->Despawn(1000,0);
			}
   }
};

class Quest_The_Ring_of_Blood_Brokentoe : public QuestScript
{
public:
	  void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
	  {
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
	  Unit * Qgiver = NULL;
				Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

				if(Qgiver)
				{
				char msg[256];
				snprintf((char*)msg, 256, "Get in the Ring of Blood, %s . The fight is about to start!", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18398, -704.669f, 7871.08f, 45.0387f, 1.59531f, true, false, 0, 0)->Despawn(600000, 0);
				}
		}
		void OnQuestComplete(Player * mTarget, QuestLogEntry * qLogEntry)
		{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		   Unit * Qgiver = NULL;
				 Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

				 if(Qgiver)
				 {
				   char msg[256];
				   snprintf((char*)msg, 256, "%s is victorious!", mTarget->GetName());
				   Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				 }

				 Unit * mogor = NULL;
				 mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

				 if(mogor)
				 {
				   char msg[256];
				   snprintf((char*)msg, 256, "...", mTarget->GetName());
				   mogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				 }
		}
};

class mogorQAI : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(mogorQAI);
	   mogorQAI(Creature* pCreature) : CreatureAIScript(pCreature)
	   {
		  _unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
	  _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	   }
};

class NotOnMyWatch : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NotOnMyWatch);
	NotOnMyWatch(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnCombatStart(Unit* mTarget)
	{
		RegisterAIUpdateEvent(1000);
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "In Nagrand, food hunt ogre!");   
	}

	void OnCombatStop()
	{
		RemoveAIUpdateEvent();
	}

	void OnDied(Unit * mTarget)
	{
		RemoveAIUpdateEvent();
	}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
		RemoveAIUpdateEvent();
	}

	void AIUpdate()
	{
		if(_unit->GetHealthPct() < 30)
		{
			Unit *plr = _unit->GetAIInterface()->GetMostHated();
			if(plr->IsPlayer())
				static_cast<Player*>(plr)->EventAttackStop();

			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
			_unit->GetAIInterface()->WipeHateList();
			_unit->GetAIInterface()->WipeTargetList();
			_unit->_setFaction();
			_unit->SetStandState(STANDSTATE_SIT);
			_unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);

			_unit->Despawn(180000, 0);

			RemoveAIUpdateEvent();
		}
	}

};

class LumpGossipScript : public GossipScript
{
public:
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem(0, "Why are Boulderfist out this far? You know this is Kurenai territory!", 1); 
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		Creature *Lump = (Creature*)(pObject);
		if (Lump == NULL)
			return;

		switch (IntId)
		{
		case 0:
			GossipHello(pObject, plr, true);
			break;
		case 1:
			if(plr->GetQuestLogForEntry(9918))
			{
				QuestLogEntry *en = plr->GetQuestLogForEntry(9918);
				if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
				{
					uint32 newcount = en->GetMobCount(0) + 1;
					en->SetMobCount(0, newcount);
					en->SendUpdateAddKill(0);
					en->UpdatePlayerFields();
				}
			}
			break;
		}
	}
 
    void Destroy()
    {
        delete this;
	}

};


// Stopping the Spread
bool StoppingTheSpread(uint32 i, Spell* pSpell)
{
  if(!pSpell->u_caster->IsPlayer())
	return true;

  Player *plr = (Player*)pSpell->u_caster;
  Creature *target = (Creature*)plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ(), 18240);

  if(target == NULL)
	  return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9874);
  
  if(qle == NULL)
    return true;
 
  GameObject *obj = NULL;
 
  if(qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
  {
		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		
		obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1);
		sEAS.GameobjectDelete(obj, 1*30*1000);
  }
  target->Despawn(2000, 60*1000);
  plr->UpdateNearbyGameObjects();
  qle->UpdatePlayerFields();
  return true;
}

//Ruthless Cunning
bool RuthlessCunning(uint32 i, Spell * pSpell)
{
	if(!pSpell->u_caster->IsPlayer())
		return true;
	
	Player *plr = (Player*)pSpell->u_caster;
	Creature * kilsorrow = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ());

	if(!kilsorrow || kilsorrow->isAlive())
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(9927);

	if(qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
	{
		kilsorrow->Despawn(0, 60000);
		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
	return true;
}

void SetupNagrand(ScriptMgr * mgr)
{
	mgr->register_creature_script(17147, &GiseldaTheCroneQAI::Create);
	mgr->register_creature_script(17148, &GiseldaTheCroneQAI::Create);
	mgr->register_creature_script(18397, &GiseldaTheCroneQAI::Create);
	mgr->register_creature_script(18658, &GiseldaTheCroneQAI::Create);
	mgr->register_creature_script(17146, &GiseldaTheCroneQAI::Create);
	mgr->register_creature_script(18351, &NotOnMyWatch::Create);
	mgr->register_creature_script(18069, &mogorQAI::Create);
	
	mgr->register_quest_script(9977, CREATE_QUESTSCRIPT(Quest_The_Ring_of_Blood_The_Final_Challenge));
	mgr->register_quest_script(9973, CREATE_QUESTSCRIPT(Quest_The_Ring_of_Blood_The_Warmaul_Champion));
	mgr->register_quest_script(9972, CREATE_QUESTSCRIPT(Quest_The_Ring_of_Blood_Skragath));
	mgr->register_quest_script(9970, CREATE_QUESTSCRIPT(Quest_The_Ring_of_Blood_Rokdar_the_Sundered_Lord));
	mgr->register_quest_script(9967, CREATE_QUESTSCRIPT(Quest_The_Ring_of_Blood_The_Blue_Brothers));
	mgr->register_quest_script(9962, CREATE_QUESTSCRIPT(Quest_The_Ring_of_Blood_Brokentoe));

	GossipScript * LumpGossip = (GossipScript*) new LumpGossipScript;
	mgr->register_gossip_script(18351, LumpGossip);
	mgr->register_dummy_spell(32146, &StoppingTheSpread);
	mgr->register_dummy_spell(32307, &RuthlessCunning);
}

