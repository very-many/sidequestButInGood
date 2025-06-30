#pragma once

#include <string>

#include "network/command_handler.h"

namespace Sidequest::Server {
    class Database;

    template <class ModelClass>
    class ReadCommand : public CommandHandler {
    public:
        explicit ReadCommand(Database* database);
        void execute(const httplib::Request& request, httplib::Response& response) override;

    protected:
        Database* database;
    };
}
