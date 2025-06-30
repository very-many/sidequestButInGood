#ifndef DATABASE_CREATOR_H
#define DATABASE_CREATOR_H
#include <string>

namespace Sidequest::Server {
    class Database;

    class DatabaseFactory {
    public:
        static Database* fetch_database(std::string& db_path, std::string& schema_path);
        static Database* reset_database(std::string& db_path, std::string& schema_path);

    private:

        static std::string open_schema_file(std::ifstream &);
        static std::ifstream try_open_filestream(const std::string &);
        static void apply_schema(const Database & database, const std::string & string);

    };
};

#endif //DATABASE_CREATOR_H
