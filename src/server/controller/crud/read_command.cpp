#include "read_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

#include "model/id.h"

namespace Sidequest::Server {
    template <class ModelClass>
    ReadCommand<ModelClass>::ReadCommand(Database* database)
        : database(database) {
    }

    template <class ModelClass>
    void ReadCommand<ModelClass>::execute(const httplib::Request& request, httplib::Response& response) {
        Id object_id = std::stoul(request.path_params.at("id"));
        std::cout << "readCMD received with: " << object_id << std::endl;
        auto domain_object = new ModelClass(database);

        try {
            domain_object->id = object_id;
            domain_object->read_on_database();
        }
        catch (UnableToReadObjectException& e) {
            response.set_content(Json("no such domain object"), "text/plain");
            response.status = httplib::StatusCode::NotFound_404;
            response.set_header("Access-Control-Allow-Origin", "*"); //TODO: for dev
            return;
        }
        std::string object_as_json = domain_object->to_json().dump();
        response.set_content(object_as_json, "application/json");
        response.set_header("Access-Control-Allow-Origin", "*"); //TODO: for dev
        response.status = httplib::StatusCode::OK_200;
    }
}

#include "model/server_user.h"
#include "model/server_quest.h"

namespace Sidequest::Server {
    template class ReadCommand<ServerUser>;
    template class ReadCommand<ServerQuest>;
}
