#pragma once

#include <string>

namespace Sidequest::Server {
    class Database;
    class ConnectionHandler;

    class ServerApplication {
    public:
        ServerApplication(const std::string& hostname, int port);
        ServerApplication(std::string hostname, int port, Database* database);
        ~ServerApplication();

        void run() const;
        void stop() const;

    protected:
        void setup_commands() const;

    protected:
        Database* database;
        bool owns_database;
        ConnectionHandler* connection_handler;
    };
}
