#include "client_application.h"

#include "httplib.h"

#include <iostream>
#include <json.hpp>

#include "network/client_stubs.h"

using json = nlohmann::json;

namespace Sidequest::Client {
    ClientApplication::ClientApplication(const std::string& hostname, int port)
        : _http_client(hostname) {
        _stubs = new Stubs(_http_client);
    }

    ClientApplication::~ClientApplication() {
        delete(_stubs);
    }

    Stubs* ClientApplication::stubs() {
        return _stubs;
    }

    httplib::Client& ClientApplication::http_connection() {
        return _http_client;
    }
}
