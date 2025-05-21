#include "server_user.h"

#include "storage/database.h"
#include "storage/query.h"

namespace Sidequest::Server {
    ServerUser::ServerUser(Database *database) : Persistable(database) {
    }

    ServerUser::ServerUser(Database *database, std::string email) : Persistable(database)
          , User(email) {
    }

    ServerUser::ServerUser(Database *database, std::string email, std::string display_name, std::string password)
        : Persistable(database)
          , User(email, display_name, password) {
    }

    ServerUser::~ServerUser() {
    }

    void ServerUser::create_on_database() {
        auto query = Query(database, "INSERT INTO user(email, display_name, password) VALUES (?, ?, ?);");
        query.bind(1, email);
        query.bind(2, display_name);
        query.bind(3, password);
        query.execute();

        if (!query.is_done())
            throw UnableToCreateObjectException(email);
    }

    void ServerUser::read_on_database() {
        auto query = Query(database, "SELECT * FROM user WHERE email = ?;");
        query.bind(1, email);
        query.execute();

        if (!query.has_row())
            throw UnableToReadObjectException(email);

        display_name = query.read_text_value("display_name");
        password = query.read_text_value("password");
    }

    void ServerUser::update_on_database() {
        auto query = Query(database, "UPDATE user set display_name=?, password=? WHERE email=?;");
        query.bind(1, display_name);
        query.bind(2, password);
        query.bind(3, email);
        query.execute();

        if (!query.is_done())
            throw UnableToUpdateObjectException(email);
    }

    void ServerUser::delete_on_database() {
        auto query = Query(database, "DELETE FROM user WHERE email=?;");
        query.bind(1, email);
        query.execute();

        if (!query.is_done())
            throw UnableToDeleteObjectException(email);
    }

    std::string ServerUser::class_id() {
        return "user";
    }
}
