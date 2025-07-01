#include "update_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest::Server {
    template <class ModelClass>
    UpdateCommand<ModelClass>::UpdateCommand(Database* database)
        : database(database) {
    }

    template <class ModelClass>
    void UpdateCommand<ModelClass>::execute(const httplib::Request& request, httplib::Response& response) {
        auto json = Json::parse(request.body);
        std::cout << "updateCMD received with: " << json.dump() << std::endl;
        auto model_object = new ModelClass(database);
        model_object->from_json(json);

        try {
            model_object->update_on_database();
        }
        catch (UnableToCreateObjectException& e) {
            response.set_content(Json("unable to update model_object"), "text/plain");
            response.status = httplib::StatusCode::BadRequest_400;
            response.set_header("Access-Control-Allow-Origin", "*"); //TODO: for dev
            return;
        }

        response.set_content(model_object->to_json().dump(), "application/json");
        response.set_header("Access-Control-Allow-Origin", "*"); //TODO: for dev
        response.status = httplib::StatusCode::OK_200;
    }
}

#include "model/server_user.h"
#include "model/server_quest.h"

namespace Sidequest::Server {
    template class UpdateCommand<ServerUser>;
    template class UpdateCommand<ServerQuest>;
}
