#include "server_user.h"

#include "storage/database.h"

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

		ServerUser::ServerUser(Database* database, std::string email, std::string display_name, std::string password)
			: Persistable(database)
			, User(email, display_name, password)
		{
		}

		ServerUser::~ServerUser()
		{
		}

		void ServerUser::_create_on_database(PreparedStatement* prepared_statement) 
		{
			database->bind(prepared_statement, 1, email);
			database->bind(prepared_statement, 2, display_name);
			database->bind(prepared_statement, 3, password);
			if (!database->execute(prepared_statement))
				throw UnableToCreateObject(email);
		}

		void ServerUser::_read_on_database( PreparedStatement* prepared_statement )
		{
			database->bind(prepared_statement, 1, email);
			if (!database->execute(prepared_statement))
				throw NoSuchDatabaseObject(email);
			display_name = database->read_text_value(prepared_statement, "display_name");
			password     = database->read_text_value(prepared_statement, "password");
		}

		void ServerUser::_update_on_database(PreparedStatement* prepared_statement) 
		{
			database->bind(prepared_statement, 1, display_name);
			database->bind(prepared_statement, 2, password);
			database->bind(prepared_statement, 3, email);
			if (!database->execute(prepared_statement))
				throw UnableToDeleteObject(email);
		}

		void ServerUser::_delete_on_database(PreparedStatement* prepared_statement) 
		{
		}

		std::string ServerUser::create_statement()
		{
			return "INSERT INTO user(email, display_name, password) VALUES (?, ?, ?);";
		}

		std::string ServerUser::read_statement()
		{
			return "SELECT * FROM user WHERE email = ?;";
		}

		std::string ServerUser::update_statement()
		{
			return "UPDATE user set display_name=?, password=? WHERE email=?;";
		}

		std::string ServerUser::delete_statement()
		{
			return "SELECT * FROM user WHERE email=?;";
		}

		std::string ServerUser::class_id()
		{
			return "user";
		}

	}
}