#pragma once

#include <string>
#include <unordered_map>

#include "prepared_statement.h"

namespace Sidequest
{
	namespace Server
	{
		class Database;

		class ColumnCache
		{
		public:
			typedef std::unordered_map<std::string, int> ColumnMap;

			ColumnCache( Database* database );
			virtual ~ColumnCache();

			ColumnMap* get_columns_of_table(std::string table_name, PreparedStatement* statement);
			ColumnMap* add_columns_of_table(std::string table_name, PreparedStatement* statement);

		protected:
			std::unordered_map<std::string, ColumnMap*> columns_by_table;
			Database* database;
		};

	};
}