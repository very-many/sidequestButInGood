#include "list_quest_command.h"

#include <string>

#include "storage/database.h"

#include "model/server_user.h"
#include "model/server_quest.h"

#include "model/id.h"

namespace Sidequest::Server {
    ListQuestCommand::ListQuestCommand(Database* database)
        : database(database) {
    }

    void ListQuestCommand::execute(const httplib::Request& request, httplib::Response& response) {
        auto query = create_query(request);
        execute_query_and_serialize_result_to_response(response, query);
        delete query;
    }

    void ListQuestCommand::execute_query_and_serialize_result_to_response(httplib::Response& response, Query* query) {
        Json result = {};

        for (auto it = query->begin(); it != query->end(); ++it) {
            try {
                auto serverQuest = ServerQuest(database);
                serverQuest.id = query->read_integer_value("id");
                serverQuest.read_from_query(*query);
                result.push_back(serverQuest.to_json());
            } catch (UnableToReadObjectException& e) {
                response.set_content(Json("no such domain object"), "application/json");
                response.status = httplib::StatusCode::NotFound_404;
                response.set_header("Access-Control-Allow-Origin", "*"); //TODO: for dev
                return;
            }
        }

        response.set_content(result.dump(), "application/json");
        response.set_header("Access-Control-Allow-Origin", "*"); //TODO: for dev
        response.status = httplib::StatusCode::OK_200;
    }

    QuestsByParentCommand::QuestsByParentCommand(Database* database)
        : ListQuestCommand(database) {
    }

    Query* QuestsByParentCommand::create_query(const httplib::Request& request) {
        Id parent_id = std::stoul(request.path_params.at("id"));
        // std::cout << "ByParentCMD received with: " << parent_id << std::endl;
        auto query = new Query(database, "SELECT * FROM quest WHERE parent=?;");
        query->bind(1, static_cast<long>(parent_id));
        return query;
    }

    MainQuestsByOwnerCommand::MainQuestsByOwnerCommand(Database* database)
        : ListQuestCommand(database) {
    }

    Query* MainQuestsByOwnerCommand::create_query(const httplib::Request& request) {
        Id owner_id = std::stoul(request.path_params.at("id"));
        // std::cout << "ByOwnerCMD received with: " << owner_id << std::endl;
        auto query = new Query(database, "SELECT * FROM quest WHERE parent IS NULL and owner = ?; ");
        query->bind(1, owner_id);
        return query;
    }
}