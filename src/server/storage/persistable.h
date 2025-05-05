#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>

#include "prepared_statement.h"

namespace Sidequest
{
	namespace Server
	{
		class Database;

		/* All persistent classes should derive from Persistable */
		class Persistable
		{
		public:
			Persistable( Database* database );
			virtual ~Persistable();

			virtual void load();
			virtual void store();
			virtual void update();

			virtual void _load( PreparedStatement* prepared_statement ) = 0;
			virtual void _store( PreparedStatement* prepared_statement ) = 0;
			virtual void _update( PreparedStatement* prepared_statement ) = 0;

			virtual std::string load_statement() = 0;
			virtual std::string store_statement() = 0;
			virtual std::string update_statement() = 0;
			virtual std::string class_id() = 0;

		protected:
			sqlite3_stmt* load_stmt = nullptr;
			Database* database;
		};

	};
}