#include "database_factory.h"
#include "database.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <io.h>

namespace Sidequest::Server {

    Database * DatabaseFactory::fetch_database(std::string &database_path, std::string &schema_path) {
        if (std::ifstream(database_path).is_open())
            return new Database(database_path);

        std::ifstream schema_file = try_open_filestream(schema_path);

        std::ofstream db_file(database_path); //create file
        auto database = new Database(database_path);

        auto schema = open_schema_file(schema_file);

        apply_schema(*database, schema);
        return database;
    }

    Database * DatabaseFactory::reset_database(std::string &database_path, std::string &schema_path) {
        std::remove(database_path.c_str());
        if (std::ifstream{database_path}.is_open())
            throw std::runtime_error("Error deleting file");
        return fetch_database(database_path, schema_path);
    }

    std::string DatabaseFactory::open_schema_file(std::ifstream &schema_file) {
        schema_file.clear();
        schema_file.seekg(0, std::ios::beg);

        std::ostringstream oss;
        oss << schema_file.rdbuf();
        auto schema = oss.str();
        schema.erase(std::remove(schema.begin(), schema.end(), '\n'), schema.end());
        return  schema;
    }

    std::ifstream DatabaseFactory::try_open_filestream(const std::string &path) {
        std::ifstream file_stream(path);
        if (!file_stream.is_open())
            throw std::runtime_error("Unable to open file");
        return file_stream;
    }

    void DatabaseFactory::apply_schema(const Database& database, const std::string &schema) {
        int status_code = database.execute(schema);
        if (status_code != SQLITE_OK)
            throw std::runtime_error("Unable to apply schema " + schema + "\nSqlite Error code: " + sqlite3_errstr(status_code));
    }
}