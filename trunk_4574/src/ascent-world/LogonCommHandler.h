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

#ifndef LOGONCOMMHANDLER_H
#define LOGONCOMMHANDLER_H

typedef struct
{
	uint32 ID;
	string Name;
	string Address;
	uint32 Port;
	uint32 ServerID;
	uint32 RetryTime;
	bool   Registered;
}LogonServer;

typedef struct
{
	string Name;
	string Address;
	uint32 Colour;
	uint32 Icon;
	uint32 TimeZone;
	float Population;
}Realm;

enum RealmType
{
	REALMTYPE_NORMAL = 0,
	REALMTYPE_PVP	= 4,
	REALMTYPE_RP	 = 6,
	REALMTYPE_RPPVP  = 8,
};

class SocketLoadBalancer;

class LogonCommHandler : public Singleton<LogonCommHandler>
{
#ifdef WIN32
	typedef HM_NAMESPACE::hash_map<string, string> ForcedPermissionMap;
#else
	typedef map<string, string> ForcedPermissionMap;
#endif

	ForcedPermissionMap forced_permissions;
	map<LogonServer*, LogonCommClientSocket*> logons;
	map<uint32, WorldSocket*> pending_logons;
	set<Realm*> realms;
	set<LogonServer*> servers;
	uint32 idhigh;
	uint32 next_request;
	Mutex mapLock;
	Mutex pendingLock;
	bool pings;
	uint32 _realmType;

public:
	uint8 sql_passhash[20];

	LogonCommHandler();
	~LogonCommHandler();

	LogonCommClientSocket * ConnectToLogon(string Address, uint32 Port);
	void UpdateAccountCount(uint32 account_id, uint8 add);
	void RequestAddition(LogonCommClientSocket * Socket);
	void CheckAllServers();
	void Startup();
	void ConnectionDropped(uint32 ID);
	void AdditionAck(uint32 ID, uint32 ServID);
	void UpdateSockets();
	void Connect(LogonServer * server);
	void ConnectAll();
	//void LogonDatabaseSQLExecute(const char* str, ...);
	//void LogonDatabaseReloadAccounts();

	void Account_SetBanned(const char * account, uint32 banned);
	void Account_SetGM(const char * account, const char * flags);
	void Account_SetMute(const char * account, uint32 muted);
	void IPBan_Add(const char * ip, uint32 duration);
	void IPBan_Remove(const char * ip);

	void LoadRealmConfiguration();
	void AddServer(string Name, string Address, uint32 Port);

	ASCENT_INLINE uint32 GetRealmType() { return _realmType; }
	void SetRealmType(uint32 type) { _realmType = type; }

	/////////////////////////////
	// Worldsocket stuff
	///////

	uint32 ClientConnected(string AccountName, WorldSocket * Socket);
	void UnauthedSocketClose(uint32 id);
	void RemoveUnauthedSocket(uint32 id);
	WorldSocket* GetSocketByRequest(uint32 id)
	{
		//pendingLock.Acquire();

		WorldSocket * sock;
		map<uint32, WorldSocket*>::iterator itr = pending_logons.find(id);
		sock = (itr == pending_logons.end()) ? 0 : itr->second;

		//pendingLock.Release();
		return sock;
	}
	ASCENT_INLINE Mutex & GetPendingLock() { return pendingLock; }		
	const string* GetForcedPermissions(string& username);

	void TestConsoleLogon(string& username, string& password, uint32 requestnum);
};

#define sLogonCommHandler LogonCommHandler::getSingleton()

#endif

