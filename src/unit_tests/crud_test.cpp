#include <gtest/gtest.h>

#include "storage/database.h"
#include "model/server_user.h"

class CRUDTests : public ::testing::Test 
{
protected:
	Sidequest::Server::Database* database;

	CRUDTests() {
	}

	virtual ~CRUDTests() {
	}

	virtual void SetUp() {
		database = new Sidequest::Server::Database("unittest.db");
	}

	virtual void TearDown() {
		delete database;
	}
};

TEST_F(CRUDTests, OPEN_DATABASE)
{
}

TEST_F(CRUDTests, CRUD_USER_CREATE)
{
	auto user = new Sidequest::Server::ServerUser( database, "temporary@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	auto user2 = new Sidequest::Server::ServerUser(database, "temporary@hs-aalen.de");
	user2->read_on_database();

	EXPECT_EQ(user->display_name, "Temporary User");
}

TEST_F(CRUDTests, CRUD_USER_READ)
{
	auto user = new Sidequest::Server::ServerUser(database, "sidequest_root@hs-aalen.de");
	user->read_on_database();

	EXPECT_EQ(user->display_name, "Sidequest Root User");
}