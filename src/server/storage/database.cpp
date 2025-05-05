#include "database.h"

#include "statement_cache.h"
#include "column_cache.h"

namespace Sidequest 
{

	namespace Server 
	{

		DatabaseNotFoundException::DatabaseNotFoundException(const std::string& message)
			: std::runtime_error(message)
		{
		}

		IncorrectSQLStatmentException::IncorrectSQLStatmentException(const std::string& message)
			: std::runtime_error(message)
		{
		}

		Database::Database( std::string url )
		{
			open( url );
			statement_cache = new StatementCache( this );
			column_cache = new ColumnCache( this );
		}

		Database::~Database()
		{
			delete(statement_cache);
			delete(column_cache);
			if ( is_open )
				close();
		}
		
		void Database::set_load_key(PreparedStatement* prepared_statement, std::string key)
		{
			if (sqlite3_bind_text(prepared_statement, 1, key.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) 
			{
				sqlite3_finalize(prepared_statement);
				throw IncorrectSQLStatmentException( "error using key " + key );
			}
		}

		void Database::open(std::string url)
		{
			int return_code = sqlite3_open( url.c_str(), &handle );
			if ( return_code  )
			{
				sqlite3_close( handle );
				throw DatabaseNotFoundException( std::string("database not found: ") + url );
			}
			is_open = true;
		}

		void Database::close()
		{
			sqlite3_close( handle );
			is_open = false;
		}

	}

}