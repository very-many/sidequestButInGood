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

		NoSuchDatabaseObject::NoSuchDatabaseObject(const std::string& message)
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
		
		bool Database::load(PreparedStatement* prepared_statement, std::string key)
		{
			if (sqlite3_bind_text(prepared_statement, 1, key.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
			{
				sqlite3_finalize(prepared_statement);
				throw IncorrectSQLStatmentException("error using key " + key);
			}
			if (sqlite3_step(prepared_statement) != SQLITE_ROW)
				return false;
			return true;
		}

		bool Database::load(PreparedStatement* prepared_statement, unsigned int key)
		{
			if (sqlite3_bind_int(prepared_statement, 1, key) != SQLITE_OK)
			{
				sqlite3_finalize(prepared_statement);
				throw IncorrectSQLStatmentException("error using key " + key);
			}
			if (sqlite3_step(prepared_statement) != SQLITE_ROW)
				return false;
			return true;
		}

		int Database::read_int_value(PreparedStatement* statement, std::string column_name)
		{
			int column_index = column_cache->get_column_index( statement, column_name);
			int result = static_cast<int>( sqlite3_column_int64( statement, column_index) );
			return result;
		}

		std::string Database::read_text_value(PreparedStatement* statement, std::string column_name)
		{
			int column_index = column_cache->get_column_index(statement, column_name);
			std::string result = reinterpret_cast<const char*>(sqlite3_column_text(statement, column_index));
			return result;
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