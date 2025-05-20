#include "database_creator.h"
#include <fstream>
#include <sstream>
#include <algorithm>

#include <io.h>
#include <iostream>

namespace Sidequest::Server {

    Database * DatabaseCreator::fetch_database(std::string &database_path, std::string &schema_path) {
        if (std::ifstream(database_path).is_open())
            return new Database(database_path);

        std::ifstream schema_file = attempt_open_filepath_as_stream(schema_path);

        std::ofstream db_file(database_path);
        auto database = new Database(database_path);

        auto schema = file_to_string(schema_file);
        schema.erase(std::remove(schema.begin(), schema.end(), '\n'), schema.end());

        apply_schema(*database, schema);
        return database;
    }

    Database * DatabaseCreator::reset_database(std::string &database_path, std::string &schema_path) {
        std::remove(database_path.c_str());
        return fetch_database(database_path, schema_path);
    }

    std::string DatabaseCreator::file_to_string(std::ifstream &schema_file) {
        schema_file.clear();
        schema_file.seekg(0, std::ios::beg);

        std::ostringstream oss;
        oss << schema_file.rdbuf();
        return oss.str();
    }

    std::ifstream DatabaseCreator::attempt_open_filepath_as_stream(const std::string &path) {
        std::ifstream filecontent_as_stream(path);
        if (!filecontent_as_stream.is_open())
            throw std::runtime_error("Unable to open schema file");
        return filecontent_as_stream;
    }

    void DatabaseCreator::apply_schema(Database& database, std::string &schema) {
        int code = database.execute(schema);
        if (code != SQLITE_OK) {
            throw std::runtime_error("Unable to apply schema " + schema + " " + sqlite3_errstr(code));
        }
    }
}