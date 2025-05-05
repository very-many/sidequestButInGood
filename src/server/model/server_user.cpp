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

		std::string ServerUser::create_statement()
		{
			return "INSERT INTO user(email, display_name, password) VALUES (?, ?, ?);";
		}

		void ServerUser::_create_on_database(PreparedStatement* prepared_statement)
		{
			database->bind(prepared_statement, 1, email);
			database->bind(prepared_statement, 2, display_name);
			database->bind(prepared_statement, 3, password);
			if (!database->execute(prepared_statement))
				throw UnableToCreateObjectException(email);
		}

		std::string ServerUser::read_statement()
		{
			return "SELECT * FROM user WHERE email = ?;";
		}

		void ServerUser::_read_on_database( PreparedStatement* prepared_statement )
		{
			database->bind(prepared_statement, 1, email);
			if (!database->execute(prepared_statement))
				throw UnableToReadObjectException(email);
			display_name = database->read_text_value(prepared_statement, "display_name");
			password     = database->read_text_value(prepared_statement, "password");
		}

		std::string ServerUser::update_statement()
		{
			return "UPDATE user set display_name=?, password=? WHERE email=?;";
		}

		void ServerUser::_update_on_database(PreparedStatement* prepared_statement)
		{
			database->bind(prepared_statement, 1, display_name);
			database->bind(prepared_statement, 2, password);
			database->bind(prepared_statement, 3, email);
			if (!database->execute(prepared_statement))
				throw UnableToUpdateObjectException(email);
		}

		std::string ServerUser::delete_statement()
		{
			return "DELETE FROM user WHERE email=?;";
		}

		void ServerUser::_delete_on_database(PreparedStatement* prepared_statement)
		{
			database->bind(prepared_statement, 1, email);
			if (!database->execute(prepared_statement))
				throw UnableToDeleteObjectException(email);
		}

		std::string ServerUser::class_id()
		{
			return "user";
		}

	}
}