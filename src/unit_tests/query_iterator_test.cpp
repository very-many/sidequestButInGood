#include <gtest/gtest.h>

#include "storage/database.h"
#include "model/server_user.h"
#include "storage/database_factory.h"
#include "storage/query.h"

class QUERY_ITERATOR_Test : public ::testing::Test {
protected:
    Sidequest::Server::Database *database = nullptr;

    QUERY_ITERATOR_Test() {}

    ~QUERY_ITERATOR_Test() override = default;

    void SetUp() override {
        // std::string db_path = "../application_root/crud_test.db";
        std::string db_path = ":memory:";
        std::string schema_path = "../application_root/create_database.sql";
        this->database = Sidequest::Server::DatabaseFactory::fetch_database(db_path, schema_path);
        populate_database();
    }

    void TearDown() override {
        delete database;
    }

    private:
        void populate_database() const {
            using namespace Sidequest::Server;
            ServerUser(database, "temp User1", "user_1@hs-aalen.de", "").create_on_database();
            ServerUser(database, "temp User2", "user_2@hs-aalen.de", "").create_on_database();
            ServerUser(database, "temp User3", "user_3@hs-aalen.de", "").create_on_database();
        }
};

using namespace Sidequest::Server;

TEST_F(QUERY_ITERATOR_Test, QUERY_ITERATOR) {
    auto query = Query(database, "SELECT * FROM user;");
    int count = 1;

    for (auto it = query.begin(); it != query.end(); ++it) {
        std::string display_name = query.read_text_value("display_name");
        EXPECT_EQ(display_name, "temp User" + std::to_string(count));
        count++;
    }

}