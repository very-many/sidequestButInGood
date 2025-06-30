#pragma once

#include <string>

#include "network/command_handler.h"

namespace Sidequest::Server {
    class Database;

    template <class ModelClass>
    class DeleteCommand : public CommandHandler {
    public:
        explicit DeleteCommand(Database* database);
        void execute(const httplib::Request& request, httplib::Response& response) override;

    protected:
        Database* database;
    };
}
