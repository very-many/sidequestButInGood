#ifndef SERVER_QUEST_H
#define SERVER_QUEST_H

#include "model/quest.h"
#include "storage/persistable.h"

namespace Sidequest::Server {
    class ServerQuest : public Quest, public Persistable {
    public:
        ServerQuest(Database *database);

        ServerQuest(Database *database, Id id);

        ~ServerQuest();

        void create_on_database() override;

        void read_on_database() override;

        void update_on_database() override;

        void delete_on_database() override;

        std::string class_id();
    };
}

#endif //SERVER_QUEST_H
