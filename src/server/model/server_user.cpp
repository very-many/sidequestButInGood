#include "server_user.h"

#include "storage/database.h"

namespace Sidequest 
{
	namespace Server {

		ServerUser::ServerUser( Database* database )
			: Persistable( database )
		{
		}

		ServerUser::~ServerUser()
		{
		}

		void ServerUser::_load( PreparedStatement* prepared_statement ) 
		{
			database->load( prepared_statement, email );
			database->read_text_value(prepared_statement, "display_name" );
			database->read_int_value(prepared_statement, "id" );
		}

		void ServerUser::_store( PreparedStatement* prepared_statement ) {
		}

		void ServerUser::_update( PreparedStatement* prepared_statement ) {
		}

		std::string ServerUser::load_statement() 
		{
			return "SELECT id, display_name, email FROM users WHERE email = ?;";
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
			return "USER";
		}

	}
}