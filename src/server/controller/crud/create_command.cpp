#include "create_command.h"

#include <string>

#include "storage/database.h"

#include "model/server_user.h"
#include "model/server_quest.h"

namespace Sidequest::Server {
    template <class ModelClass>
    CreateCommand<ModelClass>::CreateCommand(Database* database)
        : database(database) {
    }

    template <class ModelClass>
    void CreateCommand<ModelClass>::execute(const httplib::Request& request, httplib::Response& response) {
        auto json = Json::parse(request.body);
        // std::cout << "createCMD received with: " << json.dump() << std::endl;
        auto model_object = new ModelClass(database);
        model_object->from_json(json);

        try {
            model_object->create_on_database();
        } catch (UnableToCreateObjectException& e) {
            response.set_content(Json("unable to create model class"), "text/plain");
            response.set_header("Access-Control-Allow-Origin", "*"); //TODO: for dev
            response.status = httplib::StatusCode::BadRequest_400;
            return;
        }

        Json json_response = model_object->to_json();
        response.set_content(json_response.dump(), "application/json");
        response.set_header("Access-Control-Allow-Origin", "*"); //TODO: for dev
        response.status = httplib::StatusCode::Created_201;
    }
}

namespace Sidequest::Server {
    template class CreateCommand<ServerUser>;
    template class CreateCommand<ServerQuest>;
}
