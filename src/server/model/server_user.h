#pragma once

#include <string>

#include <model/user.h>
#include <storage/persistable.h>

namespace Sidequest::Server {
    class Query;

    class ServerUser : public Sidequest::User, public Persistable {
    public:
        typedef unsigned long Id;

        ServerUser(Database*, Id);

        ServerUser(Database*, const std::string&, const std::string&, const std::string&);

        ~ServerUser() override;

        void create_on_database() override;

        void read_on_database() override;

        void update_on_database() override;

        void delete_on_database() override;

        std::string class_id() override;

        void bind_all_params(Query query) const;

        void load_owned_main_quests_from_db();
    };
}
