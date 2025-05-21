#ifndef DATABASE_CREATOR_H
#define DATABASE_CREATOR_H
#include <string>
#include "database.h"

namespace Sidequest::Server {
    class DatabaseFactory {
    public:

        static Database* fetch_database(std::string&, std::string&);
        static Database* reset_database(std::string&, std::string&);

    private:

        static std::string file_to_string(std::ifstream &);
        static std::ifstream attempt_open_filepath_as_stream(const std::string &);
        static void apply_schema(Database & database, std::string & string);

    };
};

#endif //DATABASE_CREATOR_H
