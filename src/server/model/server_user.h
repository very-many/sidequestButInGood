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

			virtual void _create_on_database( PreparedStatement* ) override;
			virtual void _read_on_database(PreparedStatement*) override;
			virtual void _update_on_database(PreparedStatement*) override;
			virtual void _delete_on_database(PreparedStatement*) override;

			virtual std::string create_statement() override;
			virtual std::string read_statement() override;
			virtual std::string update_statement() override;
			virtual std::string delete_statement() override;

			virtual std::string class_id() override;
		};

	}
}
