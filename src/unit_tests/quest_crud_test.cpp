#include <gtest/gtest.h>

#include "model/server_quest.h"
#include "storage/database.h"
#include "model/server_user.h"
#include "storage/database_factory.h"
#include "model/quest.h"


class QUESTTests : public ::testing::Test {
protected:
    Sidequest::Server::Database *database = nullptr;

    QUESTTests() {
    }

    ~QUESTTests() override = default;

    void SetUp() override {
        // std::string db_path = "../../application_root/query_crud_test.db";
        std::string db_path = ":memory:";
        std::string schema_path = "../../application_root/create_database.sql";
        database = Sidequest::Server::DatabaseFactory::fetch_database(db_path, schema_path);
    }

    void TearDown() override {
        delete database;
    }
};

using namespace Sidequest::Server;

TEST_F(QUESTTests, CRUD_QUEST_CREATE) {
    using Sidequest::User;
    using Sidequest::Quest;
    const auto quest = new ServerQuest(database, "quest1", "first quest", new Quest(10), new User(2), new User(1));
    quest->create_on_database();
    const auto id = quest->id;
    delete(quest);

    const auto quest2 = new ServerQuest(database, id);
    quest2->read_on_database();

    EXPECT_EQ(quest2->name, "quest1");
    EXPECT_EQ(quest2->description, "first quest");
    EXPECT_EQ(quest2->parent->id, 10);
    EXPECT_EQ(quest2->owner->id, 2);
    EXPECT_EQ(quest2->editor->id, 1);
    delete(quest2);
}

TEST_F(QUESTTests, CRUD_QUEST_NULL_CREATE) {
    using Sidequest::User;
    using Sidequest::Quest;
    const auto quest = new ServerQuest(database, "quest1", "first quest", nullptr, nullptr, nullptr);
    quest->create_on_database();
    const auto id = quest->id;
    delete(quest);

    const auto quest2 = new ServerQuest(database, id);
    quest2->read_on_database();

    EXPECT_EQ(quest2->name, "quest1");
    EXPECT_EQ(quest2->description, "first quest");
    EXPECT_EQ(quest2->parent, nullptr);
    EXPECT_EQ(quest2->owner, nullptr);
    EXPECT_EQ(quest2->editor, nullptr);
    delete(quest2);
}

TEST_F(QUESTTests, CRUD_QUEST_UPDATE) {
    using Sidequest::User;
    using Sidequest::Quest;
    const auto quest = new ServerQuest(database, "quest1", "first quest", new Quest(10), new User(2), new User(1));
    quest->create_on_database();
    auto id = quest->id;
    quest->name = "changedName";
    quest->update_on_database();
    delete(quest);

    auto quest2 = new ServerQuest(database, id);
    quest2->read_on_database();

    EXPECT_EQ(quest2->name, "changedName");
    EXPECT_EQ(quest2->description, "first quest");
    EXPECT_EQ(quest2->parent->id, 10);
    EXPECT_EQ(quest2->owner->id, 2);
    EXPECT_EQ(quest2->editor->id, 1);
    delete(quest2);
}

TEST_F(QUESTTests, CRUD_QUEST_DELETE) {
    using Sidequest::User;
    using Sidequest::Quest;
    const auto quest = new ServerQuest(database, "quest1", "first quest", new Quest(10), new User(2), new User(1));
    quest->create_on_database();
    const auto id = quest->id;
    delete(quest);

    const auto quest2 = new ServerQuest(database, id);
    quest2->delete_on_database();
    delete(quest2);

    try {
        const auto quest3 = std::make_unique<ServerQuest>(ServerQuest(database, id));
        quest3->read_on_database();
        FAIL();
    } catch (const UnableToReadObjectException &expected) {
    }
}
