#include "server_quest.h"

#include <iostream>

#include "server_user.h"
#include "storage/query.h"
#include "storage/database.h"

namespace Sidequest::Server {
    ServerQuest::ServerQuest(Database* database)
        : Persistable(database) {
    }

    ServerQuest::ServerQuest(Database* database, const Id id) : SerialisableQuest(id), Persistable(database) {
    }

    ServerQuest::ServerQuest(Database* database, const std::string& title, const std::string& description, Quest* parent,
                             User* owner, User* editor)
        : SerialisableQuest(title, description, initial ,parent, owner, editor), Persistable(database) {
    }

    ServerQuest::ServerQuest(Database* database, const std::string& title, const std::string& description,
                             Status status, Quest* parent, User* owner, User* editor)
        : SerialisableQuest(title, description, status, parent, owner, editor), Persistable(database) {
    }

    ServerQuest::~ServerQuest() = default;

    void ServerQuest::bind_all_params(Query& query) const {
        query.bind(1, title);
        query.bind(2, description);
        query.bind(3, Quest::status_to_string(status));
        query.bind(4, parent_id);
        query.bind(5, owner_id);
        query.bind(6, editor_id);
    }

    void ServerQuest::create_on_database() {
        auto query = Query(database,
            "INSERT INTO quest(title, description, status, parent, owner, editor) VALUES (?, ?, ?, ?, ?, ?);");
        bind_all_params(query);
        query.execute();
        if (!query.is_done())
            throw UnableToCreateObjectException(std::to_string(id));

        this->id = query.last_row_id();
    }

    void ServerQuest::read_on_database() {
        auto query = Query(database, "SELECT title, description, status, parent, owner, editor FROM quest WHERE id=?;");
        query.bind(1, static_cast<long>(id));
        query.execute();

        if (!query.has_row())
            throw UnableToReadObjectException(std::to_string(id));

        read_from_query(query);
    }

    void ServerQuest::update_on_database() {
        auto query = Query(
            database, "UPDATE quest set title=?, description=?, status=?, parent=?, owner=?, editor=? WHERE id=?;");
        bind_all_params(query);
        query.bind(7, static_cast<long>(this->id));
        query.execute();
        if (!query.is_done())
            throw UnableToUpdateObjectException(std::to_string(id));
    }

    void ServerQuest::delete_on_database() {
        auto query = Query(database, "DELETE FROM quest WHERE id=?;");
        query.bind(1, static_cast<long>(this->id));
        query.execute();

        if (!query.is_done())
            throw UnableToDeleteObjectException(std::to_string(id));
    }

    void ServerQuest::read_from_query(const Query& query) {
        this->title = query.read_text_value("title");
        this->description = query.read_text_value("description");
        this->status = string_to_status(query.read_text_value("status"));
        this->parent_id = query.read_optional_integer_value("parent");
        this->owner_id = query.read_optional_integer_value("owner");
        this->editor_id = query.read_optional_integer_value("editor");
    }

    void ServerQuest::load_subquests_from_db() {
        auto query = Query(
            database, "SELECT id, title, description, status, parent, owner, editor FROM quest WHERE parent=?;");
        query.bind(1, static_cast<long>(id));

        for (auto it = query.begin(); it != query.end(); ++it) {
            const auto t_id = query.read_integer_value("id");
            if (t_id == 0)
                continue;

            auto subQuest = new ServerQuest(
                database,
                query.read_text_value("title"),
                query.read_text_value("description"),
                string_to_status(query.read_text_value("status")),
                this,
                nullptr,
                nullptr);
            subQuest->id = t_id;
            subQuest->parent_id = this->id;
            subQuest->owner_id = query.read_optional_integer_value("owner");
            subQuest->editor_id = query.read_optional_integer_value("editor");
            this->subquests.emplace_back(subQuest);
        }
    }

    void ServerQuest::load_subquests_recursive_from_db() {
        load_subquests_from_db();
        if (subquests.empty())
            return;

        for (const auto quest : this->subquests)
            if (auto* server_quest = dynamic_cast<ServerQuest*>(quest))
                server_quest->load_subquests_recursive_from_db();
    }

    std::string ServerQuest::class_id() {
        return "quest";
    }
}
