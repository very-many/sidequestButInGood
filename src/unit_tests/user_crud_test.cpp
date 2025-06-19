#include <gtest/gtest.h>

#include "storage/database.h"
#include "model/server_user.h"
#include "storage/database_factory.h"

class CRUDTests : public ::testing::Test {
protected:
    Sidequest::Server::Database* database = nullptr;

    CRUDTests() {
    }

    ~CRUDTests() override = default;

    void SetUp() override {
        std::string db_path = ":memory:"; //"../../application_root/crud_test.db";
        std::string schema_path = "../../application_root/create_database.sql";
        database = Sidequest::Server::DatabaseFactory::fetch_database(db_path, schema_path);
    }

    void TearDown() override {
        delete database;
    }
};

using namespace Sidequest::Server;

TEST_F(CRUDTests, CRUD_USER_CREATE) {
    const auto user = new ServerUser(database, "Temporary User", "crud_user_create@hs-aalen.de", "");
    user->create_on_database();
    const auto id = user->id;
    delete(user);

    const auto user2 = new ServerUser(database, id);
    user2->read_on_database();

    EXPECT_EQ(user2->display_name, "Temporary User");
    delete(user2);
}

TEST_F(CRUDTests, CRUD_USER_READ) {
    const auto user = new ServerUser(database, "Temporary User", "crud_user_create@hs-aalen.de", "");
    user->create_on_database();
    const auto id = user->id;
    delete(user);

    const auto user2 = new ServerUser(database, id);
    user2->read_on_database();

    EXPECT_EQ(user2->display_name, "Temporary User");
}

TEST_F(CRUDTests, CRUD_USER_UPDATE) {
    const auto user = new ServerUser(database, "Temporary User", "crud_user_create@hs-aalen.de", "");
    user->create_on_database();
    const auto id = user->id;
    user->display_name = "Changed Display Name";
    user->update_on_database();
    delete(user);

    const auto user2 = new ServerUser(database, id);
    user2->read_on_database();

    EXPECT_EQ(user2->display_name, "Changed Display Name");
    delete(user2);
}

TEST_F(CRUDTests, CRUD_USER_DELETE) {
    const auto user = new ServerUser(database, "Temporary User", "crud_user_create@hs-aalen.de", "");
    user->create_on_database();
    const auto id = user->id;
    delete(user);

    const auto user2 = new ServerUser(database, id);
    user2->delete_on_database();
    delete(user2);

    try {
        auto user3 = std::make_unique<ServerUser>(ServerUser(database, id));
        user3->read_on_database();
        FAIL();
    }
    catch (const UnableToReadObjectException& expected) {
    }
}
