#pragma once
#include <gtest/gtest.h>

#include <iostream>
#include <thread>

namespace Sidequest {
	class SerialisableUser;
	class SerialisableQuest;

	namespace Server {
		class Database;
		class ServerApplication;
	}

	namespace Client {
		class ClientApplication;
	}

	class RestTest : public ::testing::Test {
	protected:
		RestTest();

		~RestTest() override;

		void SetUp() override;

		void TearDown() override;

		SerialisableQuest* create_subquest(SerialisableQuest* parent_quest, SerialisableUser* owner);

		Server::Database* database;
		Server::ServerApplication* server;
		std::thread thread;
		Client::ClientApplication* client;
	};
}
