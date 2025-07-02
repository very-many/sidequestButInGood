#pragma once

#include "connection_handler.h"

#include "httplib.h"

#include <iostream>
#include <json.hpp>
#include "command_handler.h"

using json = nlohmann::json;

namespace Sidequest::Server {
    std::function<void(const httplib::Request&, httplib::Response&)> CommandHandler::get_function() {
        auto function = [this](auto && PH1, auto && PH2) { execute(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); };
        return function;
    }

    // For webclient dev on react-dev-server, allowing cross-origin-requests
    void CommandHandler::set_CORS_header(httplib::Response& response) {
        response.set_header("Access-Control-Allow-Origin", "*");
        response.set_header("Access-Control-Allow-Methods", "GET, PUT, POST, DELETE, OPTIONS");
        response.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");

        // response.set_header("Cache-Control", "no-store");
    }
}
