#include "server_user.h"

#include "model/quest.h"
#include "storage/database.h"
#include "storage/query.h"

namespace Sidequest::Server {
    ServerUser::ServerUser(Database* database, Id id) : User(id), Persistable(database) {
    }

    ServerUser::ServerUser(Database* database, const std::string& display_name, const std::string& email,
                           const std::string& password)
        : User(display_name, email, password), Persistable(database) {
    }

    ServerUser::ServerUser(Database* database, std::string&& display_name, std::string&& email, std::string&& password)
        : User(display_name, email, password), Persistable(database) {
    }

    ServerUser::~ServerUser() = default;

    void ServerUser::bind_all_params(Query query) const {
        query.bind(1, this->display_name);
        query.bind(2, this->email);
        query.bind(3, this->password);
    }

    void ServerUser::create_on_database() {
        auto query = Query(database, "INSERT INTO user(display_name, email, password) VALUES (?, ?, ?);");
        bind_all_params(query);
        query.execute();
        this->id = query.last_row_id();

        if (!query.is_done())
            throw UnableToCreateObjectException(this->email);
    }

    void ServerUser::read_on_database() {
        auto query = Query(database, "SELECT * FROM user WHERE id=?;");
        query.bind(1, static_cast<int>(this->id));
        query.execute();

        if (!query.has_row())
            throw UnableToReadObjectException(email);

        display_name = query.read_text_value("display_name");
        password = query.read_text_value("password");
    }

    void ServerUser::update_on_database() {
        auto query = Query(database, "UPDATE user set display_name=?, email=?, password=? WHERE id=?;");
        bind_all_params(query);
        query.bind(4, static_cast<int>(this->id));
        query.execute();

        if (!query.is_done())
            throw UnableToUpdateObjectException(email);
    }

    void ServerUser::delete_on_database() {
        auto query = Query(database, "DELETE FROM user WHERE id=?;");
        query.bind(1, static_cast<int>(this->id));
        query.execute();

        if (!query.is_done())
            throw UnableToDeleteObjectException(this->email);
    }

    void ServerUser::load_owned_main_quests_from_db() {
        auto query = Query(database, "SELECT name, description, parent, owner, editor FROM quest WHERE owner=? and parent IS NULL;");
        query.bind(1, static_cast<long>(id));

        for (auto it = query.begin(); it != query.end(); ++it) {
            auto quest = new Quest(query.read_text_value("name"),
                                query.read_text_value("description"),
                                new Quest(query.read_integer_value("parent")),
                                this,
                                new User(query.read_integer_value("editor")));
            this->main_quests.push_back(quest);
        }
    }

    std::string ServerUser::class_id() {
        return "user";
    }
}
