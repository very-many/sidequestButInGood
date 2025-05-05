#include <gtest/gtest.h>

#include "storage/database.h"
#include "model/server_user.h"

TEST(UnitTests, OPEN_DATABASE)
{
	auto database = new Sidequest::Server::Database("unittest.db");
	delete database;
}

TEST(UnitTests, CRUD_USER_LOAD) 
{
	auto database = new Sidequest::Server::Database("unittest.db");
	auto user = new Sidequest::Server::ServerUser( database, "sidequest_root@hs-aalen.de" );
	user->load();

	EXPECT_EQ(user->display_name, "Sidequest Root User");
}