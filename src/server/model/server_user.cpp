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
			database->set_load_key( prepared_statement, email );
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