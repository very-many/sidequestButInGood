#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>

#include "prepared_statement.h"

/* Base class of all persistent domain model objects on the server
*  that implement CRUDS operations */
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

			virtual void create_on_database();
			virtual void read_on_database();
			virtual void update_on_database();
			virtual void delete_on_database();

			virtual std::string create_statement() = 0;
			virtual void _create_on_database(PreparedStatement* prepared_statement) = 0;

			virtual std::string read_statement() = 0;
			virtual void _read_on_database( PreparedStatement* prepared_statement ) = 0;

			virtual std::string update_statement() = 0;
			virtual void _update_on_database(PreparedStatement* prepared_statement) = 0;

			virtual std::string delete_statement() = 0;
			virtual void _delete_on_database(PreparedStatement* prepared_statement) = 0;

			virtual std::string class_id() = 0;

		protected:
			sqlite3_stmt* load_stmt = nullptr;
			Database* database;
		};

	};
}