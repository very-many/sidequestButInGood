#ifndef SERVER_QUEST_H
#define SERVER_QUEST_H

#include "model/quest.h"
#include "network/serialisable_quest.h"
#include "storage/persistable.h"
#include "storage/query.h"

namespace Sidequest::Server {
    class ServerQuest : public SerialisableQuest, public Persistable {
    public:
        ServerQuest(Database* database, Id id);
        explicit ServerQuest(Database* database);
        ServerQuest(Database* database, const std::string& title, const std::string& description, Quest* parent = nullptr,
                    User* owner = nullptr, User* editor = nullptr);
        ServerQuest(Database* database, const std::string& title, const std::string& description, Status status,
                    Quest* parent = nullptr, User* owner = nullptr, User* editor = nullptr);
        ~ServerQuest() override;

        void create_on_database() override;
        void read_on_database() override;
        void update_on_database() override;
        void delete_on_database() override;

        void read_from_query(const Query& query);
        void load_subquests_from_db();
        void load_subquests_recursive_from_db();

        std::string class_id() override;
    private:
        void bind_all_params(Query& query) const;
    };
}
#endif //SERVER_QUEST_H