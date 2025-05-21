
#include "server_quest.h"
#include "storage/query.h"

namespace Sidequest::Server {
    ServerQuest::ServerQuest(Database* database) : Persistable(database) {}

    ServerQuest::ServerQuest(Database *database, Id id) : Persistable(database), Quest(id) {
    }

    ServerQuest::~ServerQuest() = default;

    //TODO: which parameter does quest have?

    void ServerQuest::create_on_database() {
        auto query = Query(database, "INSERT INTO quest(id, caption, parent_id) VALUES (?, ?, ?);");
        //TODO table to link subquests
        query.bind(1, id);
        query.bind(2, caption);
        query.bind(3, parent->id);
        query.execute();
        if (!query.is_done())
            throw UnableToCreateObjectException(std::to_string(id));
    }


    void ServerQuest::read_on_database() {
        auto query = Query(database, "SELECT * FROM quest WHERE id = ?;");
        query.bind(1, id);
        query.execute();

        if (!query.has_row())
            throw UnableToReadObjectException(std::to_string(id));

        caption = query.read_text_value("caption");
        //TODO: load parent
        //TODO: allow different loading levels for subquests, just by id or full load
    }

    void ServerQuest::update_on_database() {
        auto query = Query(database, "UPDATE quest set caption=?, parent=?, subquests=? WHERE id = ?;");
        query.bind(1, caption);
        query.bind(2, parent->id);
        query.bind(3, id); //TODO: subquests
        query.bind(4, id);
        query.execute();

        if (!query.is_done())
            throw UnableToUpdateObjectException(std::to_string(id));
    }

    //TODO: allow to delete recursive
    void ServerQuest::delete_on_database() {
        auto query = Query(database, "DELETE FROM user WHERE id=?;");
        query.bind(1, id);
        query.execute();

        if (!query.is_done())
            throw UnableToDeleteObjectException(std::to_string(id));
    }

    std::string ServerQuest::class_id() {
        return "quest";
    }
}
