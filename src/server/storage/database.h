#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <sqlite3.h>

#include "prepared_statement.h"

namespace Sidequest
{
	namespace Server
	{
		class Persistable;
		class StatementCache;
		class ColumnCache;

		class DatabaseNotFoundException : public std::runtime_error
		{
		public:
			explicit DatabaseNotFoundException(const std::string& message);
		};

		class IncorrectSQLStatmentException : public std::runtime_error
		{
		public:
			explicit IncorrectSQLStatmentException(const std::string& statement);
		};

		class Database
		{
		public:
			Database( std::string url );
			~Database();

			void set_load_key(PreparedStatement* prepared_statement, std::string key);
			void set_load_key(PreparedStatement* prepared_statement, unsigned int key);

			StatementCache* statement_cache;
			ColumnCache* column_cache;

		protected:
			void open( std::string url );
			void close();

		protected:
			bool is_open = false;
			sqlite3* handle = nullptr;

			friend class StatementCache;
		};

	};
}