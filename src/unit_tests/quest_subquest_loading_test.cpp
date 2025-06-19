#include <gtest/gtest.h>

#include "model/server_quest.h"
#include "storage/database.h"
#include "model/server_user.h"
#include "storage/database_factory.h"
#include "storage/query.h"

class QUEST_TEST : public ::testing::Test {
protected:
    Sidequest::Server::Database* database = nullptr;
    Sidequest::Server::ServerQuest::Id parent_id = 0;
    std::vector<Sidequest::Quest*> quests = {};

    QUEST_TEST() {
    }

    ~QUEST_TEST() override {
        for (const auto quest : quests)
            delete quest;
    }

    void SetUp() override {
        // std::string db_path = "../../application_root/crud_test.db";
        std::string db_path = ":memory:";
        std::string schema_path = "../../application_root/create_database.sql";
        this->database = Sidequest::Server::DatabaseFactory::fetch_database(db_path, schema_path);
        this->parent_id = 0;
        populate_database();
    }

    void TearDown() override {
        delete database;
    }

    static void sort_quest_list(std::vector<Sidequest::Quest*>& quests) {
        std::sort(quests.begin(), quests.end(),
            [](const auto& lhs, const auto& rhs) {
                return lhs->name < rhs->name;
            });
    }

    static void all_quests_as_list(Sidequest::Quest* quest, std::vector<Sidequest::Quest*>& out_list) {
        out_list.push_back(quest);
        sort_quest_list(quest->subquests);
        for (auto* subQ : quest->subquests)
            all_quests_as_list(subQ, out_list);
    }

private:
    /*
    q1 {
        s1 {
            ss1{}
        },
        s2 {}
    }
    */
    void populate_database() {
        using namespace Sidequest::Server;
        const auto parentQuest = new ServerQuest(database, "q1", "mainQ1", nullptr, nullptr, nullptr);
        parentQuest->create_on_database();
        quests.push_back(parentQuest);
        this->parent_id = parentQuest->id;
        const auto subQuest = new ServerQuest(database, "s1", "subQ1", parentQuest, nullptr, nullptr);
        subQuest->create_on_database();
        quests.push_back(subQuest);
        const auto subsubQuest = new ServerQuest(database, "ss1", "subsubQ1", subQuest, nullptr, nullptr);
        subsubQuest->create_on_database();
        quests.push_back(subsubQuest);
        const auto subtQuest1 = new ServerQuest(database, "s2", "subQ2", parentQuest, nullptr, nullptr);
        subtQuest1->create_on_database();
        quests.push_back(subtQuest1);
    }
};

using namespace Sidequest::Server;

TEST_F(QUEST_TEST, QUEST_LOAD_CHILDREN) {
    const auto parent_quest = new ServerQuest(database, parent_id);
    parent_quest->read_on_database();
    parent_quest->load_subquests_from_db();

    EXPECT_EQ(parent_quest->subquests.size(), 2);

    sort_quest_list(parent_quest->subquests);

    int count = 1;
    for (const auto subquest : parent_quest->subquests) {
        EXPECT_EQ(subquest->parent->id, this->parent_id);
        auto expect_name = std::string{"s" + std::to_string(count)};
        EXPECT_EQ(subquest->name, expect_name);
        auto expect_desc = std::string{"subQ" + std::to_string(count)};
        EXPECT_EQ(subquest->description, expect_desc);
        count++;
    }
}

TEST_F(QUEST_TEST, QUEST_LOAD_CHILDREN_RECURSIVE) {
    auto parent_quest = new ServerQuest(database, parent_id);
    parent_quest->read_on_database();
    parent_quest->load_subquests_recursive_from_db();

    EXPECT_EQ(parent_quest->subquests.size(), 2);

    std::vector<Sidequest::Quest*> allQ;
    all_quests_as_list(parent_quest, allQ);
    EXPECT_EQ(allQ.size(), 4);

    for (unsigned int i = 0; i < allQ.size(); i++) {
        EXPECT_EQ(allQ[i]->id, i + 1);
        EXPECT_EQ(allQ[i]->name, quests[i]->name);
    }
}