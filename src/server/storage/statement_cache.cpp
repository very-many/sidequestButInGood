#include "statement_cache.h"

#include "database.h"

namespace Sidequest 
{

	namespace Server 
	{

		StatementCache::StatementCache(Database* database)
			: database(database)
		{
		}

		StatementCache::~StatementCache()
		{
		}

		PreparedStatement* StatementCache::get_statement(std::string statement_key)
		{
			auto statement = prepared_statements.find(statement_key);
			if (statement == prepared_statements.end())
				return nullptr;
			return statement->second;
		}

		PreparedStatement* StatementCache::add_statement(std::string statement_key, std::string statement_sql)
		{
			PreparedStatement* statement;
			if (sqlite3_prepare_v2(database->handle, statement_sql.c_str(), -1, &statement, nullptr) != SQLITE_OK)
			{
				throw IncorrectSQLStatmentException( statement_sql );
			}
			prepared_statements[statement_key] = statement;
			return statement;
		}

	}

}