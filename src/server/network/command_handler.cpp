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
}
