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

    //TODO for dev
    void ConnectionHandler::register_options_command() const {
        auto optFunc = [](const httplib::Request& req, httplib::Response& res) {
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With");

            res.status = httplib::StatusCode::OK_200;
        };
        server->Options("/api/v1/quests", optFunc);
        server->Options("/api/v1/quests/:id", optFunc);
        server->Options("/api/v1/users", optFunc);
        server->Options("/api/v1/users/:id", optFunc);
    }

    void ConnectionHandler::listen() const {
        server->listen(hostname, port);
    }

    void ConnectionHandler::stop() const {
        server->stop();
    }
}
