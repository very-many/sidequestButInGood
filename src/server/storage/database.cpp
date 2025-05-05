#include "database.h"

#include "statement_cache.h"
#include "column_cache.h"

#include <iostream>

namespace Sidequest 
{

	namespace Server 
	{

		DatabaseNotFoundException::DatabaseNotFoundException(const std::string& message)
			: std::runtime_error(message)
		{
		}

		IncorrectSQLStatmentException::IncorrectSQLStatmentException(const std::string& message, int error_code)
			: std::runtime_error(message), error_code(error_code)
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
			int error_code = sqlite3_bind_text(prepared_statement, 1, key.c_str(), -1, SQLITE_TRANSIENT);
			if (error_code != SQLITE_OK)
			{
				sqlite3_finalize(prepared_statement);
				throw IncorrectSQLStatmentException("error using key " + key, error_code);
			}
			if (sqlite3_step(prepared_statement) != SQLITE_ROW)
				return false;
			return true;
		}

		bool Database::load(PreparedStatement* prepared_statement, unsigned int key)
		{
			int error_code = sqlite3_bind_int(prepared_statement, 1, key);
			if (error_code != SQLITE_OK)
			{
				sqlite3_finalize(prepared_statement);
				throw IncorrectSQLStatmentException("error using key " + key, error_code);
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
			auto c_str = reinterpret_cast<const char*>( sqlite3_column_text(statement, column_index) );
			std::cout << "RESULT: " << c_str << std::endl;
			std::string result( c_str );
			return result;
		}

		void Database::open(std::string url)
		{
			int return_code = sqlite3_open( url.c_str(), &handle );
			if ( return_code != SQLITE_OK )
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