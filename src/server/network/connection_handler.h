#pragma once

#include <string>
#include <unordered_map>

namespace httplib {
    class Server;
}


namespace Sidequest::Server {
    class CommandHandler;

    class ConnectionHandler {
    public:
        ConnectionHandler(std::string hostname, int port);
        ~ConnectionHandler();

        void register_get_command(const std::string& endpoint, CommandHandler* command) const;
        void register_put_command(const std::string& endpoint, CommandHandler* command) const;
        void register_post_command(const std::string& endpoint, CommandHandler* command) const;
        void register_delete_command(const std::string& endpoint, CommandHandler* command) const;


        //TODO: for dev
        void register_options_command() const;

        void listen() const;
        void stop() const;

    protected:
        std::unordered_map<std::string, CommandHandler*> endpoints;
        httplib::Server* server;
        std::string hostname;
        int port;
    };
}
