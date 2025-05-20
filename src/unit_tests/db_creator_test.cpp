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

TEST_F(DBTests, RESET_EXISTING_DB) {
    std::string db_path = R"(../../application_root/test_reset.db)";
    std::string schema_path = R"(../../application_root/create_database.sql)";
    auto database = DatabaseCreator::reset_database(db_path, schema_path);

    EXPECT_NE(database, nullptr);
}

TEST_F(DBTests, MISSING_SCHEMA_FILE) {
    try {
        std::string db_path = R"(../../application_root/test_missing_schema_file.db)";
        std::string schema_path = R"(../../application_root/this_does_not_exist.sql)";
        auto database = DatabaseCreator::fetch_database(db_path, schema_path);
    }
    catch (const std::runtime_error &e) {
        EXPECT_STREQ(e.what(), "Unable to open schema file");
    }
}