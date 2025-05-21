#ifndef QUERY_H
#define QUERY_H
#include "database.h"
#include "statement_cache.h"

namespace Sidequest::Server {
    class Query {
    public:
        Query(Database *database, const std::string &statement_sql);

        ~Query();

        void bind(int parameter_index, const std::string &value);

        void bind(int parameter_index, unsigned int value);

        void execute();

        [[nodiscard]] int read_int_value(const std::string &column_name) const;

        [[nodiscard]] std::string read_text_value(const std::string &column_name) const;

        [[nodiscard]] bool has_row() const;

        [[nodiscard]] bool is_done() const;

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
        Database *database = nullptr;
        PreparedStatement *prepared_statement = nullptr;
        int status_code = 0;

        [[nodiscard]] PreparedStatement *prepare(const std::string &statement_sql) const;

        [[nodiscard]] bool is_ok() const;

        void reset_statement() const;
    };
};

#endif //QUERY_H
