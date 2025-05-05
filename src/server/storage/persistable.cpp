#include "persistable.h"

#include "database.h"
#include "statement_cache.h"

namespace Sidequest 
{

	namespace Server 
	{

		Persistable::Persistable( Database* database )
		{
			this->database = database;
		}

		Persistable::~Persistable()
		{
		}

		void Persistable::read_on_database()
		{
			std::string statement_key = class_id() + "_read";
			PreparedStatement* prepared_statement = database->statement_cache->get_statement(statement_key);
			if (prepared_statement == nullptr )
			{
				auto statement_sql = read_statement();
				prepared_statement = database->statement_cache->add_statement(statement_key, statement_sql);
			}
			_read_on_database( prepared_statement );
		}

		void Persistable::create_on_database()
		{
			std::string statement_key = class_id() + "_create";
			PreparedStatement* prepared_statement = database->statement_cache->get_statement(statement_key);
			if (prepared_statement == nullptr)
			{
				auto statement_sql = create_statement();
				prepared_statement = database->statement_cache->add_statement(statement_key, statement_sql);
			}
			_create_on_database(prepared_statement);
		}

		void Persistable::update_on_database()
		{
			std::string statement_key = class_id() + "_update";
			PreparedStatement* prepared_statement = database->statement_cache->get_statement(statement_key);
			if (prepared_statement == nullptr)
			{
				auto statement_sql = update_statement();
				prepared_statement = database->statement_cache->add_statement(statement_key, statement_sql);
			}
			_update_on_database(prepared_statement);
		}

		void Persistable::delete_on_database()
		{
			std::string statement_key = class_id() + "_delete";
			PreparedStatement* prepared_statement = database->statement_cache->get_statement(statement_key);
			if (prepared_statement == nullptr)
			{
				auto statement_sql = delete_statement();
				prepared_statement = database->statement_cache->add_statement(statement_key, statement_sql);
			}
			_delete_on_database(prepared_statement);
		}

	}

}