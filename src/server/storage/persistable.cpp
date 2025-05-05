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

		void Persistable::load()
		{
			std::string statement_key = class_id() + "_load";
			PreparedStatement* prepared_statement = database->statement_cache->get_statement(statement_key);
			if (prepared_statement == nullptr )
			{
				auto statement_sql = load_statement();
				prepared_statement = database->statement_cache->add_statement(statement_key, statement_sql);
			}
			_load( prepared_statement );
		}

		void Persistable::store()
		{
			std::string statement_key = class_id() + "_store";
			PreparedStatement* prepared_statement = database->statement_cache->get_statement(statement_key);
			if (prepared_statement == nullptr)
			{
				auto statement_sql = store_statement();
				prepared_statement = database->statement_cache->add_statement(statement_key, statement_sql);
			}
			_store(prepared_statement);
		}

		void Persistable::update()
		{
			std::string statement_key = class_id() + "_update";
			PreparedStatement* prepared_statement = database->statement_cache->get_statement(statement_key);
			if (prepared_statement == nullptr)
			{
				auto statement_sql = update_statement();
				prepared_statement = database->statement_cache->add_statement(statement_key, statement_sql);
			}
			_update(prepared_statement);
		}

	}

}