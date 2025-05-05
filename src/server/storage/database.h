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
			explicit IncorrectSQLStatmentException(const std::string& statement, int error_code);
			int error_code;
		};

		class NoSuchDatabaseObject : public std::runtime_error
		{
		public:
			explicit NoSuchDatabaseObject(const std::string& key);
		};

		class Database
		{
		public:
			Database( std::string url );
			~Database();

			bool load(PreparedStatement* prepared_statement, std::string key);
			bool load(PreparedStatement* prepared_statement, unsigned int key);

			int read_int_value(PreparedStatement* prepared_statement, std::string column_name);
			std::string read_text_value(PreparedStatement* prepared_statement, std::string column_name);

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