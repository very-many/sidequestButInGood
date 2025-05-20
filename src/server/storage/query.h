#ifndef QUERY_H
#define QUERY_H
#include "database.h"
#include "statement_cache.h"

namespace Sidequest::Server {
    class Query {
    public:
        Query(Database *database, std::string statement_sql);

        ~Query();

        void bind(int parameter_index, std::string value) const;

        void bind(int parameter_index, unsigned int value) const;

        int execute() const;

        int next() const;

        void reset_statement() const;

        int read_int_value(const std::string &column_name) const;

        std::string read_text_value(const std::string &column_name) const;


        class Iterator {
        public:
            explicit Iterator(Query *query, bool is_end = false);

            Iterator &operator++();

            bool operator!=(const Iterator &other) const;

        private:
            Query *query;
            bool is_end;
        };

        Iterator begin();

        Iterator end();

    private:
        Database *database;
        PreparedStatement *prepared_statement;

        PreparedStatement *prepare(const std::string &statement_sql) const;
    };
};

#endif //QUERY_H
