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

		void ServerUser::_create_on_database(PreparedStatement* prepared_statement) {
		}

		void ServerUser::_read_on_database( PreparedStatement* prepared_statement )
		{
			std::cout << "preparing statement to load " << email << std::endl;
			if (!database->load(prepared_statement, email))
				throw NoSuchDatabaseObject(email);
			display_name = database->read_text_value(prepared_statement, "display_name" );
		}

		void ServerUser::_update_on_database( PreparedStatement* prepared_statement ) {
		}

		std::string ServerUser::create_statement()
		{
			return "INSERT INTO user(email, display_name) VALUES (?, ?);";
		}

		std::string ServerUser::read_statement()
		{
			return "SELECT * FROM user WHERE email = ?;";
		}

		std::string ServerUser::update_statement()
		{
			return "";
		}

		std::string ServerUser::delete_statement()
		{
			return "SELECT * FROM user WHERE email = ?;";
		}

		std::string ServerUser::class_id()
		{
			return "user";
		}

	}
}