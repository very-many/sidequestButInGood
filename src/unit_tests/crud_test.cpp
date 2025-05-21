#include <gtest/gtest.h>

#include "storage/database.h"
#include "model/server_user.h"
#include "storage/database_factory.h"


class CRUDTests : public ::testing::Test {
protected:
    Sidequest::Server::Database *database;

    CRUDTests() {
    }

    virtual ~CRUDTests() {
    }

    virtual void SetUp() {
        std::string db_path = ":memory:"; //"../../application_root/crud_test.db";
        std::string schema_path = "../../application_root/create_test_db.sql";
        database = Sidequest::Server::DatabaseFactory::fetch_database(db_path, schema_path);
    }

    virtual void TearDown() {
        delete database;
    }
};

using namespace Sidequest::Server;

TEST_F(CRUDTests, OPEN_DATABASE) {
}

TEST_F(CRUDTests, CRUD_USER_CREATE) {
    auto user = new ServerUser(database, "crud_user_create@hs-aalen.de", "Temporary User", "");
    user->create_on_database();
    delete(user);

    auto user2 = new ServerUser(database, "crud_user_create@hs-aalen.de");
    user2->read_on_database();

    EXPECT_EQ(user2->display_name, "Temporary User");
    delete(user2);
}

TEST_F(CRUDTests, CRUD_USER_CREATE_DOUBLE) {
    auto user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User", "");
    user->create_on_database();
    delete(user);

    try {
        auto user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User 2", "");
        user->create_on_database();
        FAIL();
    } catch (const UnableToCreateObjectException &expected) {
        EXPECT_STREQ(expected.what(), "UnableToCreateObject: crud_user_create_double@hs-aalen.de");
        delete(user);
    }
}

TEST_F(CRUDTests, CRUD_USER_READ) {
    auto user = new ServerUser(database, "crud_user_read@hs-aalen.de", "Temporary User", "");
    user->create_on_database();
    delete(user);

    user = new ServerUser(database, "crud_user_read@hs-aalen.de");
    user->read_on_database();

    EXPECT_EQ(user->display_name, "Temporary User");
}

TEST_F(CRUDTests, CRUD_USER_UPDATE) {
    auto user = new ServerUser(database, "crud_user_update@hs-aalen.de", "Temporary User", "");
    user->create_on_database();
    user->display_name = "Changed Display Name";
    user->update_on_database();
    delete(user);

    auto user2 = new ServerUser(database, "crud_user_update@hs-aalen.de");
    user2->read_on_database();

    EXPECT_EQ(user2->display_name, "Changed Display Name");
    delete(user2);
}

TEST_F(CRUDTests, CRUD_USER_DELETE) {
    auto user = new ServerUser(database, "crud_user_delete@hs-aalen.de", "Temporary User", "");
    user->create_on_database();
    delete(user);

    auto user2 = new ServerUser(database, "crud_user_delete@hs-aalen.de");
    user2->delete_on_database();
    delete(user2);

    try {
        auto user3 = new ServerUser(database, "crud_user_delete@hs-aalen.de");
        user3->read_on_database();
        FAIL();
    } catch (const UnableToReadObjectException &expected) {
        delete(user);
    }
}
