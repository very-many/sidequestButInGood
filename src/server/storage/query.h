#ifndef QUERY_H
#define QUERY_H
#include <optional>

#include "statement_cache.h"
#include "model/quest.h"

namespace Sidequest::Server {
    class Database;
    class ServerQuest;

    class Query {
    public:
        Query(Database* database, const std::string& statement_sql);

        ~Query();

        void bind(int parameter_index, const std::string& value);
        void bind(int parameter_index, long value);
        void bind(int parameter_index, const std::optional<long>& value);
        void bind_null(int parameter_index);

        [[nodiscard]] long read_integer_value(const std::string& column_name) const;
        [[nodiscard]] std::optional<long> read_optional_integer_value(const std::string& column_name) const;
        [[nodiscard]] std::string read_text_value(const std::string& column_name) const;

        void execute();
        [[nodiscard]] bool has_row() const;
        [[nodiscard]] bool is_done() const;
        [[nodiscard]] Id last_row_id() const;

        class Iterator {
        public:
            explicit Iterator(Query* query, bool is_end = false);

            Iterator& operator++();

            bool operator!=(const Iterator& other) const;

        private:
            Query* query;
            bool is_end;
        };

        Iterator begin();

        Iterator end();

    private:
        int status_code = -1;
        Database* database;
        PreparedStatement* prepared_statement;

        [[nodiscard]] bool is_ok() const;
    };
};

#endif //QUERY_H
