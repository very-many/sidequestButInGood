#pragma once

#include <string>
#include <vector>

#include <model/user.h>
#include <storage/persistable.h>

namespace Sidequest
{
	namespace Server
	{

		class ServerUser : public Sidequest::User, public Persistable {
		public:
			typedef unsigned long Id;

			ServerUser(Database* database);
			ServerUser(Database* database, std::string email);
			~ServerUser();

			virtual void _load( PreparedStatement* ) override;
			virtual void _store( PreparedStatement* ) override;
			virtual void _update( PreparedStatement* ) override;

			virtual std::string load_statement() override;
			virtual std::string store_statement() override;
			virtual std::string update_statement() override;
			virtual std::string class_id() override;
		};

	}
}