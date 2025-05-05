#include "column_cache.h"

#include "database.h"

namespace Sidequest 
{

	namespace Server 
	{

		ColumnCache::ColumnCache(Database* database)
			: database(database)
		{
		}

		ColumnCache::~ColumnCache()
		{
		}

		ColumnCache::ColumnMap* ColumnCache::get_columns_of_table( std::string table_name, PreparedStatement* statement )
		{
			auto entry = columns_by_table.find(table_name);
			if (entry == columns_by_table.end())
				add_columns_of_table( table_name, statement );
			return entry->second;
		}

		ColumnCache::ColumnMap* ColumnCache::add_columns_of_table(std::string table_name, PreparedStatement* statement)
		{
			auto column_indices = new std::unordered_map<std::string, int>();
			int columnCount = sqlite3_column_count( statement );
			for (int i = 0; i < columnCount; ++i) {
				const char* name = sqlite3_column_name( statement, i );
				if (name) {
					(*column_indices)[std::string( name ) ] = i;
				}
			}
			return column_indices;
		}

	}

}