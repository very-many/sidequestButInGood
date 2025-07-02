#pragma once

#include <string>

#include "httplib.h"

namespace Sidequest::Server {
    typedef httplib::Request Request;
    typedef httplib::Response Response;

    class CommandHandler {
    public:
        virtual ~CommandHandler() = default;
        virtual void execute(const Request& request, Response& response) = 0;

        static void set_CORS_header(httplib::Response& response);

        std::function<void(const Request&, Response&)> get_function();
    };
}
