/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

void WorldSession::HandleSetVisibleRankOpcode(WorldPacket& recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 4);
	uint32 ChosenRank;
	recv_data >> ChosenRank; 
	if(ChosenRank == 0xFFFFFFFF)
		_player->SetUInt32Value(PLAYER_CHOSEN_TITLE, 0);
	else
		_player->SetUInt32Value(PLAYER_CHOSEN_TITLE, ChosenRank);
}

void HonorHandler::AddHonorPointsToPlayer(Player *pPlayer, uint32 uAmount)
{
	if( pPlayer->GetMapId() == 559 || pPlayer->GetMapId() == 562 || pPlayer->GetMapId() == 572)
		return;
	pPlayer->m_honorPoints += uAmount;
	pPlayer->m_honorToday += uAmount;
	
	pPlayer->HandleProc(PROC_ON_GAIN_EXPIERIENCE, pPlayer, NULL);
	pPlayer->m_procCounter = 0;

	RecalculateHonorFields(pPlayer);
}

int32 HonorHandler::CalculateHonorPointsForKill( Player *pPlayer, Unit* pVictim )
{
	// this sucks.. ;p
	if( pVictim == NULL )
	{
		int32 pts = rand() % 100 + 100;
		return pts;
	}

	// Suicide lol
	if( pVictim == pPlayer )
		return 0;

	if( pVictim->GetTypeId() != TYPEID_PLAYER )
		return 0;

	// How dishonorable, you fiend!
	if( pVictim->HasActiveAura( PLAYER_HONORLESS_TARGET_SPELL ) )
		return 0;

	uint32 k_level = pPlayer->GetUInt32Value( UNIT_FIELD_LEVEL );
	uint32 v_level = pVictim->GetUInt32Value( UNIT_FIELD_LEVEL );

	int k_honor = pPlayer->m_honorPoints;
	int v_honor = static_cast< Player* >( pVictim )->m_honorPoints;

	uint32 k_grey = 0;

	if( k_level > 5 && k_level < 40 )
	{
		k_grey = k_level - 5 - float2int32( floor( ((float)k_level) / 10.0f ) );
	}
	else
	{
		k_grey = k_level - 1 - float2int32( floor( ((float)k_level) / 5.0f ) );
	}

	if( k_honor == 0 )
		k_honor = 1;

	float diff_level = ((float)v_level - k_grey) / ((float)k_level - k_grey);
	if( diff_level > 2 ) diff_level = 2.0f;
	if( diff_level < 0 ) diff_level = 0.0f;

	float diff_honor = ((float)v_honor) / ((float)k_honor);
	if( diff_honor > 3 ) diff_honor = 3.0f;
	if( diff_honor < 0 ) diff_honor = 0.0f;

	float honor_points = diff_level * ( 150.0f + diff_honor * 60 );
	honor_points *= ((float)k_level) / 70.0f;
	honor_points *= World::getSingleton().getRate( RATE_HONOR );

	return float2int32( honor_points );
}

void HonorHandler::OnPlayerKilledUnit( Player *pPlayer, Unit* pVictim )
{
	if( pVictim == NULL || pPlayer == NULL )
		return;

	if( pPlayer->GetTypeId() != TYPEID_PLAYER || !pVictim->IsUnit() )
		return;

	if( !pVictim->IsPlayer() || static_cast< Player* >( pVictim )->m_honorless )
		return;

    if( pVictim->IsPlayer() )
	{
		if( pPlayer->m_bg )
		{
			if( static_cast< Player* >( pVictim )->m_bgTeam == pPlayer->m_bgTeam )
				return;

			// patch 2.4, players killed >50 times in battlegrounds won't be worth honor for the rest of that bg
			if( static_cast<Player*>(pVictim)->m_bgScore.Deaths >= 50 )
				return;
		}
		else
		{
			if( pPlayer->GetTeam() == static_cast< Player* >( pVictim )->GetTeam() )
				return;
		}
	}

	// Calculate points
	int32 Points = CalculateHonorPointsForKill(pPlayer, pVictim);

	if( Points > 0 )
	{
		if( pPlayer->m_bg )
		{
			// hackfix for battlegrounds (since the gorups there are disabled, we need to do this manually)
			vector<Player*> toadd;
			uint32 t = pPlayer->m_bgTeam;
			toadd.reserve(15);		// shouldnt have more than this
			pPlayer->m_bg->Lock();
			set<uint32> * s = &pPlayer->m_bg->m_players[t];

			for(set<uint32>::iterator itr = s->begin(); itr != s->end(); ++itr)
			{
				Player *plr = objmgr.GetPlayer(*itr);
				if( plr != NULL && (plr == pPlayer || plr->isInRange(pPlayer,100.0f)) )
					toadd.push_back(plr);
			}

			if( toadd.size() > 0 )
			{
				uint32 pts = Points / (uint32)toadd.size();
				for(vector<Player*>::iterator vtr = toadd.begin(); vtr != toadd.end(); ++vtr)
				{
					AddHonorPointsToPlayer(*vtr, pts);

					(*vtr)->m_killsToday++;
					(*vtr)->m_killsLifetime++;
					pPlayer->m_bg->HookOnHK(*vtr);
					if(pVictim)
					{
						// Send PVP credit
						WorldPacket data(SMSG_PVP_CREDIT, 12);
						uint32 pvppoints = pts * 10;
						data << pvppoints << pVictim->GetGUID() << uint32(static_cast< Player* >(pVictim)->GetPVPRank());
						(*vtr)->GetSession()->SendPacket(&data);
					}
				}
			}

			pPlayer->m_bg->Unlock();
		}
		else
		{
			set<Player*> contributors;
			// First loop: Get all the people in the attackermap.
			for(std::set<Object*>::iterator itr = pVictim->GetInRangeOppFactsSetBegin(); itr != pVictim->GetInRangeOppFactsSetEnd(); itr++)
			{
				if(!(*itr)->IsPlayer())
					continue;

				bool added = false;
				Player * plr = (Player*)(*itr);
				if(pVictim->CombatStatus.m_attackers.find(plr->GetGUID()) != pVictim->CombatStatus.m_attackers.end())
				{
					added = true;
					contributors.insert(plr);
				}

				if(added && plr->GetGroup())
				{
					Group * pGroup = plr->GetGroup();
					uint32 groups = pGroup->GetSubGroupCount();
					for(uint32 i = 0; i < groups; i++)
					{
						SubGroup * sg = pGroup->GetSubGroup(i);
						if(!sg) continue;

						for(GroupMembersSet::iterator itr2 = sg->GetGroupMembersBegin(); itr2 != sg->GetGroupMembersEnd(); itr2++)
						{
							PlayerInfo * pi = (*itr2);
							Player * gm = objmgr.GetPlayer(pi->guid);
							if(!gm) continue;

							if(gm->isInRange(pVictim, 100.0f))
								contributors.insert(gm);
						}
					}
				}
			}

			for(set<Player*>::iterator itr = contributors.begin(); itr != contributors.end(); itr++)
			{
				Player * pAffectedPlayer = (*itr);
				if(!pAffectedPlayer) continue;

				pAffectedPlayer->m_killsToday++;
				pAffectedPlayer->m_killsLifetime++;
				if(pAffectedPlayer->m_bg)
					pAffectedPlayer->m_bg->HookOnHK(pAffectedPlayer);

				int32 contributorpts = int32(Points / contributors.size());
				AddHonorPointsToPlayer(pAffectedPlayer, contributorpts);
				if(pVictim->IsPlayer())
				{
					sHookInterface.OnHonorableKill(pAffectedPlayer, (Player*)pVictim);

					WorldPacket data(SMSG_PVP_CREDIT, 12);
					uint32 pvppoints = contributorpts * 10; // Why *10?
					data << pvppoints << pVictim->GetGUID() << uint32(static_cast< Player* >(pVictim)->GetPVPRank());
					pAffectedPlayer->GetSession()->SendPacket(&data);
				}

				if(pAffectedPlayer->GetZoneId() == 3518)
				{
					// Add Halaa Battle Token
					SpellEntry * pvp_token_spell = dbcSpell.LookupEntry(pAffectedPlayer->GetTeam()? 33004 : 33005);
					pAffectedPlayer->CastSpell(pAffectedPlayer, pvp_token_spell, true);
				}
				// If we are in Hellfire Peninsula
				if(pAffectedPlayer->GetZoneId() == 3483)
				{
					// Add Mark of Thrallmar/Honor Hold
					SpellEntry * pvp_token_spell = dbcSpell.LookupEntry(pAffectedPlayer->GetTeam()? 32158 : 32155);
					pAffectedPlayer->CastSpell(pAffectedPlayer, pvp_token_spell, true);
				}
				// If we are in Zangarmarsh
				if(pPlayer->GetZoneId() == 3521)
				{
					// Add Mark of Thrallmar/Honor Hold
					SpellEntry * pvp_token_spell = dbcSpell.LookupEntry(pAffectedPlayer->GetTeam()? 32158 : 32155);
					pAffectedPlayer->CastSpell(pAffectedPlayer, pvp_token_spell, true);
				}
			}
		}
	}
}

void HonorHandler::RecalculateHonorFields(Player *pPlayer)
{
	// Why are we multiplying by 10.. ho well
	pPlayer->SetUInt32Value(PLAYER_FIELD_KILLS, pPlayer->m_killsToday);
	pPlayer->SetUInt32Value(PLAYER_FIELD_TODAY_CONTRIBUTION, pPlayer->m_honorToday *10 );
	//isnt it yesterday kills? TODO: check if PLAYER_FIELD_BYTES2 fits here.
	pPlayer->SetUInt32Value(PLAYER_FIELD_YESTERDAY_CONTRIBUTION, pPlayer->m_killsYesterday *10 /*| ( (pPlayer->m_honorYesterday * 10) << 16)*/);
	pPlayer->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS, pPlayer->m_killsLifetime);
	pPlayer->SetUInt32Value(PLAYER_FIELD_HONOR_CURRENCY, pPlayer->m_honorPoints);
	pPlayer->SetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY, pPlayer->m_arenaPoints);
}

bool ChatHandler::HandleAddKillCommand(const char* args, WorldSession* m_session)
{
	uint32 KillAmount = args ? atol(args) : 1;
	Player *plr = getSelectedChar(m_session, true);
	if(plr == 0)
		return true;

	BlueSystemMessage(m_session, "Adding %u kills to player %s.", KillAmount, plr->GetName());
	GreenSystemMessage(plr->GetSession(), "You have had %u honor kills added to your character.", KillAmount);

	for(uint32 i = 0; i < KillAmount; ++i)
		HonorHandler::OnPlayerKilledUnit(plr, 0);

	return true;
}

bool ChatHandler::HandleAddHonorCommand(const char* args, WorldSession* m_session)
{
	uint32 HonorAmount = args ? atol(args) : 1;
	Player *plr = getSelectedChar(m_session, true);
	if(plr == 0)
		return true;

	BlueSystemMessage(m_session, "Adding %u honor to player %s.", HonorAmount, plr->GetName());
	GreenSystemMessage(plr->GetSession(), "You have had %u honor points added to your character.", HonorAmount);

	HonorHandler::AddHonorPointsToPlayer(plr, HonorAmount);
	return true;
}

bool ChatHandler::HandlePVPCreditCommand(const char* args, WorldSession* m_session)
{
	uint32 Rank, Points;
	if(sscanf(args, "%u %u", (unsigned int*)&Rank, (unsigned int*)&Points) != 2)
	{
		RedSystemMessage(m_session, "Command must be in format <rank> <points>.");
		return true;
	}
	Points *= 10;
	uint64 Guid = m_session->GetPlayer()->GetSelection();
	if(Guid == 0)
	{
		RedSystemMessage(m_session, "A selection of a unit or player is required.");
		return true;
	}

	BlueSystemMessage(m_session, "Building packet with Rank %u, Points %u, GUID "I64FMT".", 
		Rank, Points, Guid);

	WorldPacket data(SMSG_PVP_CREDIT, 12);
	data << Points << Guid << Rank;
	m_session->SendPacket(&data);
	return true;
}

bool ChatHandler::HandleGlobalHonorDailyMaintenanceCommand(const char* args, WorldSession* m_session)
{
	return false;
}

bool ChatHandler::HandleNextDayCommand(const char* args, WorldSession* m_session)
{
	return false;
}
