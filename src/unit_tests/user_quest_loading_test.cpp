#include <gtest/gtest.h>

#include "model/server_quest.h"
#include "storage/database.h"
#include "model/server_user.h"
#include "model/user.h"
#include "storage/database_factory.h"
#include "storage/query.h"

class USER_QUEST_TEST : public ::testing::Test {
protected:
    Sidequest::Server::Database* database = nullptr;
    Sidequest::Server::ServerQuest::Id owner_id = 0;

    USER_QUEST_TEST() {
    }

    ~USER_QUEST_TEST() override = default;

    void SetUp() override {
        // std::string db_path = "../../application_root/crud_test.db";
        std::string db_path = ":memory:";
        std::string schema_path = "../../application_root/create_database.sql";
        this->database = Sidequest::Server::DatabaseFactory::fetch_database(db_path, schema_path);
        this->owner_id = 0;
        populate_database();
    }

    void TearDown() override {
        delete database;
    }

private:
    void populate_database() {
        using namespace Sidequest::Server;

        const auto owner = new ServerUser(database, "u1", "u1@email.com", "u1PW");
        owner->create_on_database();
        owner_id = owner->id;

        const auto parentQuest = new ServerQuest(database, "q1", "mainQ1", nullptr, owner, nullptr);
        parentQuest->create_on_database();
        ServerQuest(database, "s1", "subQ1", parentQuest, owner, nullptr).create_on_database();
        ServerQuest(database, "s2", "subQ2", parentQuest, owner, nullptr).create_on_database();
        ServerQuest(database, "s3", "subQ3", parentQuest, owner, nullptr).create_on_database();
        delete(parentQuest);

        ServerQuest(database, "q2", "mainQ2", nullptr, owner, nullptr).create_on_database();
        ServerQuest(database, "q3", "mainQ3", nullptr, owner, nullptr).create_on_database();
        ServerQuest(database, "q4", "mainQ4", nullptr, nullptr, nullptr).create_on_database();
    }
};

using namespace Sidequest::Server;

TEST_F(USER_QUEST_TEST, USER_LOAD_QUESTS) {
    const auto owner = new ServerUser(database, owner_id);
    owner->load_owned_main_quests_from_db();

    EXPECT_EQ(owner->main_quests.size(), 3);

    std::sort(owner->main_quests.begin(), owner->main_quests.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs->name < rhs->name;
        });

    int count = 1;
    for (const auto quest : owner->main_quests) {
        EXPECT_EQ(quest->owner->id, this->owner_id);
        auto expect_name = std::string{"q" + std::to_string(count)};
        EXPECT_EQ(quest->name, expect_name);
        count++;
    }
}
