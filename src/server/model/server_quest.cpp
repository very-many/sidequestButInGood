#include "server_quest.h"

#include <utility>

#include "server_user.h"
#include "storage/query.h"
#include "storage/database.h"

namespace Sidequest::Server {
    ServerQuest::ServerQuest(Database* database, const Id id) : Quest(id), Persistable(database) {
    }

    ServerQuest::ServerQuest(Database* database, const std::string& name, const std::string& description, Quest* parent,
                             User* owner, User* editor)
        : Quest(name, description, parent, owner, editor), Persistable(database) {
    }

    ServerQuest::ServerQuest(Database* database, const std::string& name, const std::string& description,
                             Status status, Quest* parent, User* owner, User* editor)
        : Quest(name, description, status, parent, owner, editor), Persistable(database) {
    }

    ServerQuest::~ServerQuest() = default;

    void ServerQuest::bind_all_params(Query& query) const {
        query.bind(1, name);
        query.bind(2, description);
        query.bind(3, Quest::status_to_string(status));
        if (parent != nullptr)
            query.bind(4, static_cast<long>(parent->id));
        else
            query.bind_null(4);
        if (owner != nullptr)
            query.bind(5, static_cast<long>(owner->id));
        else
            query.bind_null(5);
        if (editor != nullptr)
            query.bind(6, static_cast<long>(editor->id));
        else
            query.bind_null(6);
    }

    void ServerQuest::create_on_database() {
        auto query = Query(
            database, "INSERT INTO quest(name, description, status, parent, owner, editor) VALUES (?, ?, ?, ?, ?, ?);");
        bind_all_params(query);
        query.execute();
        if (!query.is_done())
            throw UnableToCreateObjectException(std::to_string(id));

        this->id = query.last_row_id();
    }

    void ServerQuest::read_on_database() {
        auto query = Query(database, "SELECT name, description, status, parent, owner, editor FROM quest WHERE id=?;");
        query.bind(1, static_cast<long>(id));
        query.execute();

        if (!query.has_row())
            throw UnableToReadObjectException(std::to_string(id));

        this->name = query.read_text_value("name");
        this->description = query.read_text_value("description");
        this->status = string_to_status(query.read_text_value("status"));
        this->parent_id = query.read_optional_integer_value("parent");
        this->owner_id = query.read_optional_integer_value("owner");
        this->editor_id = query.read_optional_integer_value("editor");
        // const auto parent_id = query.read_integer_value("parent");
        // this->parent = parent_id != 0 ? new ServerQuest(database, parent_id) : nullptr;
        // const auto owner_id = query.read_integer_value("owner");
        // this->owner = owner_id != 0 ? new User(owner_id) : nullptr;
        // const auto editor_id = query.read_integer_value("editor");
        // this->editor = editor_id != 0 ? new User(editor_id) : nullptr;
    }

    void ServerQuest::update_on_database() {
        auto query = Query(
            database, "UPDATE quest set name=?, description=?, status=?, parent=?, owner=?, editor=? WHERE id=?;");
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

    void ServerQuest::load_subquests_from_db() {
        auto query = Query(
            database, "SELECT id, name, description, status, parent, owner, editor FROM quest WHERE parent=?;");
        query.bind(1, static_cast<long>(id));

        for (auto it = query.begin(); it != query.end(); ++it) {
            auto t_id = query.read_integer_value("id");
            if (t_id == 0)
                continue;

            auto subQuest = new ServerQuest(
                database,
                query.read_text_value("name"),
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
