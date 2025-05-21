#include <fstream>
#include <gtest/gtest.h>

#include "storage/database_factory.h"
#include <cstdio>
#include <iostream>

class DbFactoryTests : public ::testing::Test {
protected:
    Sidequest::Server::Database *database = nullptr;

    DbFactoryTests() {
    }

    virtual ~DbFactoryTests() {
        delete database;
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
        delete database;
    }
};

using namespace Sidequest::Server;

TEST_F(DbFactoryTests, FETCH_DB) {
    std::string db_path =  R"(../../application_root/test.db)";
    std::remove(db_path.c_str());
    std::string schema_path = R"(../../application_root/create_database.sql)";
    const auto database = DatabaseFactory::fetch_database(db_path, schema_path);
    EXPECT_NE(database, nullptr);
    delete database;
}

TEST_F(DbFactoryTests, FETCH_EXISTING_DB) {
    std::string db_path = R"(../../application_root/sidequest.db)";
    std::string schema_path = R"(../../application_root/create_database.sql)";
    const auto database = DatabaseFactory::fetch_database(db_path, schema_path);

    EXPECT_NE(database, nullptr);
}

TEST_F(DbFactoryTests, RESET_EXISTING_DB) {
    std::string db_path = R"(../../application_root/test_reset.db)";
    std::string schema_path = R"(../../application_root/create_database.sql)";
    const auto database = DatabaseFactory::reset_database(db_path, schema_path);

    EXPECT_NE(database, nullptr);
}

TEST_F(DbFactoryTests, MISSING_SCHEMA_FILE) {
    try {
        std::string db_path = R"(../../application_root/test_missing_schema_file.db)";
        std::string schema_path = R"(../../application_root/this_does_not_exist.sql)";
        auto database = DatabaseFactory::fetch_database(db_path, schema_path);
        FAIL();
    }
    catch (const std::runtime_error &e) {
        EXPECT_STREQ(e.what(), "Unable to open schema file");
    }
}