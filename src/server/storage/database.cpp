#include "database.h"

#include "statement_cache.h"
#include "column_cache.h"


namespace Sidequest::Server {
    DatabaseNotFoundException::DatabaseNotFoundException(const std::string &message)
        : std::runtime_error(message) {
    }

    ParameterBindException::ParameterBindException(const std::string &message, int error_code)
        : std::runtime_error(message), error_code(error_code) {
    }

    UnableToCreateObjectException::UnableToCreateObjectException(const std::string &key)
        : std::runtime_error("UnableToCreateObject: " + key) {
    }

    UnableToReadObjectException::UnableToReadObjectException(const std::string &key)
        : std::runtime_error("UnableToReadObjectException: " + key) {
    }

    UnableToUpdateObjectException::UnableToUpdateObjectException(const std::string &key)
        : std::runtime_error("UnableToUpdateObjectException: " + key) {
    }

    UnableToDeleteObjectException::UnableToDeleteObjectException(const std::string &key)
        : std::runtime_error("UnableToDeleteObjectException: " + key) {
    }

    Database::Database(const std::string &filepath_of_database) {
        open(filepath_of_database);
        statement_cache = new StatementCache(this);
        column_cache = new ColumnCache(this);
    }

    Database::~Database() {
        delete(statement_cache);
        delete(column_cache);
        if (is_open)
            close();
    }

    void Database::open(const std::string &url) {
        int return_code = sqlite3_open(url.c_str(), &handle);
        if (return_code != SQLITE_OK) {
            sqlite3_close(handle);
            throw DatabaseNotFoundException(std::string("database not found: ") + url);
        }
        is_open = true;
    }

    void Database::close() {
        sqlite3_close(handle);
        is_open = false;
    }

    int Database::execute(const std::string &sql_statement) const {
        int code = sqlite3_exec(handle, sql_statement.c_str(), nullptr, nullptr, nullptr);
        return code;
    }
}
