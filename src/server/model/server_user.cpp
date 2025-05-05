#include "server_user.h"

#include "storage/database.h"

#include <iostream>

namespace Sidequest 
{
	namespace Server {

		ServerUser::ServerUser(Database* database)
			: Persistable(database)
		{
		}		
		
		ServerUser::ServerUser(Database* database, std::string email)
			: Persistable(database)
			, User(email)
		{
		}

		ServerUser::~ServerUser()
		{
		}

		void ServerUser::_load( PreparedStatement* prepared_statement ) 
		{
			std::cout << "preparing statement to load " << email << std::endl;
			if (!database->load(prepared_statement, email))
				throw NoSuchDatabaseObject(email);
			display_name = database->read_text_value(prepared_statement, "display_name" );
		}

		void ServerUser::_store( PreparedStatement* prepared_statement ) {
		}

		void ServerUser::_update( PreparedStatement* prepared_statement ) {
		}

		std::string ServerUser::load_statement() 
		{
			return "SELECT * FROM user WHERE email = ?;";
		}

		std::string ServerUser::store_statement()
		{
			return "";
		}

		std::string ServerUser::update_statement()
		{
			return "";
		}

		std::string ServerUser::class_id()
		{
			return "user";
		}

	}
}