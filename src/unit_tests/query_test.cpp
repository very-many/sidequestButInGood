#include <gtest/gtest.h>

#include "storage/database.h"
#include "model/server_user.h"
#include "storage/database_factory.h"
#include "storage/query.h"


class QUERYTest : public ::testing::Test {
protected:
    Sidequest::Server::Database *database;

    QUERYTest() {
    }

    virtual ~QUERYTest() {
    }

    virtual void SetUp() {
        std::string db_path = ":memory:"; //"../../application_root/crud_test.db";
        std::string schema_path = "../../application_root/create_test_db.sql";
        this->database = Sidequest::Server::DatabaseFactory::fetch_database(db_path, schema_path);
        populate_database();
    }

    virtual void TearDown() {
        delete database;
    }

    private:
        void populate_database() const {
            using namespace Sidequest::Server;
            ServerUser(database, "user_1@hs-aalen.de", "temp User1", "").create_on_database();
            ServerUser(database, "user_2@hs-aalen.de", "temp User2", "").create_on_database();
            ServerUser(database, "user_3@hs-aalen.de", "temp User3", "").create_on_database();
        }
};

using namespace Sidequest::Server;

TEST_F(QUERYTest, QUERY_ITERATOR) {
    auto query = Query(database, "SELECT * FROM user;");
    int count = 1;

    for (auto it = query.begin(); it != query.end(); ++it) {
        std::string display_name = query.read_text_value("display_name");
        EXPECT_EQ(display_name, "temp User" + std::to_string(count));
        count++;
    }
}


