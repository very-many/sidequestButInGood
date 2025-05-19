#include <fstream>
#include <gtest/gtest.h>

#include "storage/database_creator.h"
#include <cstdio>
#include <iostream>

class DBTests : public ::testing::Test {
protected:
    Sidequest::Server::Database *database;

    DBTests() {
    }

    virtual ~DBTests() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

using namespace Sidequest::Server;

TEST_F(DBTests, FILE_TO_STRING) {
    auto stream = std::ifstream(R"(../../application_root/create_database.sql)");
    std::string s = DatabaseCreator::file_to_string(stream);
    std::cout << s;
    EXPECT_EQ(s, "create table user(email text primary key, display_name text, password text);");
}

TEST_F(DBTests, FETCH_DB) {
    std::string db_path =  R"(../../application_root/test.db)";
    std::remove(db_path.c_str());
    std::string schema_path = R"(../../application_root/create_database.sql)";
    auto database = DatabaseCreator::fetch_database(db_path, schema_path);
    EXPECT_NE(database, nullptr);
    delete database;
}

TEST_F(DBTests, FETCH_EXISTING_DB) {
    std::string db_path = R"(../../application_root/sidequest.db)";
    std::string schema_path = R"(../../application_root/create_database.sql)";
    auto database = DatabaseCreator::fetch_database(db_path, schema_path);

    EXPECT_NE(database, nullptr);
}

TEST_F(DBTests, MISSING_SCHEMA_FILE){}