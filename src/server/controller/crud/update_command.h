#pragma once

#include <string>

#include "network/command_handler.h"

namespace Sidequest::Server {
    class Database;

    template <class ModelClass>
    class UpdateCommand : public CommandHandler {
    public:
        explicit UpdateCommand(Database* database);
        void execute(const httplib::Request& request, httplib::Response& response) override;

    protected:
        Database* database;
    };
}
