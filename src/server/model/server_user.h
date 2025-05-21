#pragma once

#include <string>
#include <vector>

#include <model/user.h>
#include <storage/persistable.h>

namespace Sidequest::Server {
    class ServerUser : public Sidequest::User, public Persistable {
    public:
        typedef unsigned long Id;

        ServerUser(Database *database);

        ServerUser(Database *database, std::string email);

        ServerUser(Database *database, std::string email, std::string display_name, std::string password);

        ~ServerUser();

        void create_on_database() override;

        void read_on_database() override;

        void update_on_database() override;

        void delete_on_database() override;

        std::string class_id() override;
    };
}
