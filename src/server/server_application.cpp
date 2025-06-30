#include <server_application.h>

#include <iostream>
#include <chrono>
#include <thread>

#include "storage/database.h"
#include "network/connection_handler.h"

#include "model/server_user.h"
#include "model/server_quest.h"

#include "controller/crud/read_command.h"
#include "controller/crud/create_command.h"
#include "controller/crud/update_command.h"
#include "controller/crud/delete_command.h"
#include "storage/database_factory.h"

#include "view/list_quest_command.h"

namespace Sidequest::Server {
    ServerApplication::ServerApplication(const std::string& hostname, int port) {
        connection_handler = new ConnectionHandler(hostname, port);
        // database = new Sidequest::Server::Database("sidequest.db");
        std::string db_path = R"(../application_root/sidequest.db)";
        std::string schema_path = R"(../application_root/create_database.sql)";
        database = DatabaseFactory::fetch_database(db_path, schema_path);
        owns_database = true;
        setup_commands();
    }

    ServerApplication::ServerApplication(std::string hostname, int port, Database* database)
        : database(database) {
        connection_handler = new ConnectionHandler(hostname, port);
        owns_database = false;
        setup_commands();
    }

    ServerApplication::~ServerApplication() {
        if (owns_database)
            delete(database);
        delete(connection_handler);
    }

    void ServerApplication::run() const {
        connection_handler->listen();
    }

    void ServerApplication::stop() const {
        connection_handler->stop();
    }

    void ServerApplication::setup_commands() const {
        connection_handler->register_put_command("/api/user/create", new CreateCommand<ServerUser>(database));
        connection_handler->register_get_command("/api/user/:id/read", new ReadCommand<ServerUser>(database));
        connection_handler->register_put_command("/api/user/:id/update", new UpdateCommand<ServerUser>(database));
        connection_handler->register_delete_command("/api/user/:id/delete", new DeleteCommand<ServerUser>(database));

        connection_handler->register_put_command("/api/quest/create", new CreateCommand<ServerQuest>(database));
        connection_handler->register_get_command("/api/quest/:id/read", new ReadCommand<ServerQuest>(database));
        connection_handler->register_put_command("/api/quest/:id/update", new UpdateCommand<ServerQuest>(database));
        connection_handler->register_delete_command("/api/quest/:id/delete", new DeleteCommand<ServerQuest>(database));

        connection_handler->register_get_command("/api/quest/byparent/:id/", new QuestsByParentCommand(database));
        connection_handler->register_get_command("/api/quest/byowner/:id/", new MainQuestsByOwnerCommand(database));
    }
}
