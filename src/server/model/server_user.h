#pragma once

#include <optional>
#include <string>
#include "network/serialisable_user.h"
#include "storage/persistable.h"

namespace Sidequest::Server {
    class Query;

    class ServerUser : public SerialisableUser, public Persistable {
    public:
        typedef unsigned long Id;

        ServerUser(Database*, Id id);
        ServerUser(Database*);

        ServerUser(Database* database, const std::string& display_name, const std::string& email, const std::string& password);

        ServerUser(Database* database, std::string&& display_name, std::string&& email, std::string&& password);

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
