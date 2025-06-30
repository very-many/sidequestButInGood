#pragma once

#include "connection_handler.h"

#include "httplib.h"

#include "network/command_handler.h"

namespace Sidequest::Server {
    ConnectionHandler::ConnectionHandler(std::string hostname, int port)
        : hostname(hostname), port(port) {
        server = new httplib::Server();
    }

    ConnectionHandler::~ConnectionHandler() {
        for (auto& it : endpoints)
            delete(it.second);
    }

    void ConnectionHandler::register_get_command(const std::string& endpoint, CommandHandler* command) const {
        auto function = command->get_function();
        server->Get(endpoint, function);
    }

    void ConnectionHandler::register_put_command(const std::string& endpoint, CommandHandler* command) const {
        const auto function = command->get_function();
        server->Put(endpoint, function);
    }

    void ConnectionHandler::register_post_command(const std::string& endpoint, CommandHandler* command) const {
        const auto function = command->get_function();
        server->Post(endpoint, function);
    }

    void ConnectionHandler::register_delete_command(const std::string& endpoint, CommandHandler* command) const {
        const auto function = command->get_function();
        server->Delete(endpoint, function);
    }

    void ConnectionHandler::listen() const {
        server->listen(hostname, port);
    }

    void ConnectionHandler::stop() const {
        server->stop();
    }
}
