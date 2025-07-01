#include "delete_command.h"

#include <string>

#include "model/id.h"
#include "model/server_user.h"
#include "storage/database.h"


namespace Sidequest::Server {
    template <class ModelClass>
    DeleteCommand<ModelClass>::DeleteCommand(Database* database)
        : database(database) {
    }

    template <class ModelClass>
    void DeleteCommand<ModelClass>::execute(const httplib::Request& request, httplib::Response& response) {
        Id object_id = std::stoul(request.path_params.at("id"));
        // std::cout << "deleteCMD received with: " << object_id << std::endl;
        auto model_object = new ModelClass(database);
        model_object->id = object_id;

        try {
            model_object->delete_on_database();
        }
        catch (UnableToCreateObjectException& e) {
            response.set_content(Json("unable to delete model_object"), "text/plain");
            response.status = httplib::StatusCode::BadRequest_400;
            response.set_header("Access-Control-Allow-Origin", "*"); //TODO: for dev
            return;
        }

        response.set_content("", "text/plain");
        response.set_header("Access-Control-Allow-Origin", "*"); //TODO: for dev
        response.status = httplib::StatusCode::NoContent_204;
    }
}

#include "model/server_user.h"
#include "model/server_quest.h"

namespace Sidequest::Server {
    template class DeleteCommand<ServerUser>;
    template class DeleteCommand<ServerQuest>;
}
