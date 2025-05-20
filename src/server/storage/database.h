#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

#include "prepared_statement.h"

namespace Sidequest::Server {
    class Persistable;
    class StatementCache;
    class ColumnCache;

    class DatabaseNotFoundException : public std::runtime_error {
    public:
        explicit DatabaseNotFoundException(const std::string &message);
    };

    class ParameterBindException : public std::runtime_error {
    public:
        explicit ParameterBindException(const std::string &statement, int error_code);

        int error_code;
    };

    class UnableToReadObjectException : public std::runtime_error {
    public:
        explicit UnableToReadObjectException(const std::string &key);
    };

    class UnableToCreateObjectException : public std::runtime_error {
    public:
        explicit UnableToCreateObjectException(const std::string &key);
    };

    class UnableToUpdateObjectException : public std::runtime_error {
    public:
        explicit UnableToUpdateObjectException(const std::string &key);
    };

    class UnableToDeleteObjectException : public std::runtime_error {
    public:
        explicit UnableToDeleteObjectException(const std::string &key);
    };

    class Database {
    public:
        Database(const std::string &filepath_of_database);

        ~Database();

        StatementCache *statement_cache;
        ColumnCache *column_cache;

        int execute(const std::string &sql_statement) const;


    protected:
        void open(const std::string &filepath_of_database);

        void close();

    protected:
        bool is_open = false;
        sqlite3 *handle = nullptr;

        friend class StatementCache;
    };
};