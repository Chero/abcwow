/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#if !defined(DATABASEENV_H)
#define DATABASEENV_H

/************************************************************************/
/* Database Support Setup											   */
/************************************************************************/
// Define the databases that you would like the server to be compiled with here.

#define DATABASE_SUPPORT_MYSQL

//! Other libs we depend on.
#include "Common.h"
#include "Singleton.h"
#include "Log.h"
#include "CThreads.h"

//! Our own includes.
#include <mysql/mysql.h>
#include "Database/DBCStores.h"
#include "Database/Field.h"
#include "Database/Database.h"

#endif