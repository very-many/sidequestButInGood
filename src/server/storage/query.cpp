#include "query.h"

#include <utility>
#include "column_cache.h"

namespace Sidequest::Server {
    Query::Query(Database *database, std::string statement_sql) {
        this->database = database;
        prepared_statement = prepare(std::move(statement_sql));
    }

    Query::~Query() {
        reset_statement();
        //TODO: FIGMA wann finalize?
        //sqlite3_finalize(prepared_statement);
    }

    PreparedStatement *Query::prepare(const std::string &statement_sql) const {
        PreparedStatement *prepared_statement = database->statement_cache->get_statement(statement_sql);
        if (prepared_statement == nullptr)
            prepared_statement = database->statement_cache->add_statement(statement_sql);
        return prepared_statement;
    }

    int Query::execute() const {
        return next();
    }

    int Query::next() const {
        const int return_code = sqlite3_step(prepared_statement);
        if (return_code == SQLITE_ROW || return_code == SQLITE_DONE)
            return return_code;
        throw ParameterBindException("error executing query", return_code);
    }


    void Query::bind(int parameter_index, std::string value) const {
        int error_code = sqlite3_bind_text(prepared_statement, parameter_index, value.c_str(), -1, SQLITE_TRANSIENT);
        if (error_code != SQLITE_OK)
            throw ParameterBindException("error binding parameter " + std::to_string(parameter_index) + " to " + value,
                                         error_code);
    }

    void Query::bind(int parameter_index, const unsigned int value) const {
        int error_code = sqlite3_bind_int(prepared_statement, parameter_index, value);
        if (error_code != SQLITE_OK) {
            throw ParameterBindException(
                "error binding parameter " + std::to_string(parameter_index) + " to " + std::to_string(value),
                error_code);
        }
    }

    void Query::reset_statement() const {
        sqlite3_reset(prepared_statement);
    }

    int Query::read_int_value(const std::string &column_name) const {
        int column_index = database->column_cache->get_column_index(prepared_statement, column_name);
        int result = static_cast<int>(sqlite3_column_int64(prepared_statement, column_index));
        return result;
    }

    std::string Query::read_text_value(const std::string &column_name) const {
        int column_index = database->column_cache->get_column_index(prepared_statement, column_name);
        auto c_str = reinterpret_cast<const char *>(sqlite3_column_text(prepared_statement, column_index));
        std::string result(c_str);
        return result;
    }

    void Query::free() {
        //TODO: FIGMA remove from statement cache
        //database->statement_cache->remove_statement(prepared_statement);
        sqlite3_finalize(prepared_statement);

    }


    Query::Iterator::Iterator(Query *query, bool is_end) : query(query), is_end(is_end) {
        if (!is_end) {
            int return_code = sqlite3_step(query->prepared_statement);
            if (return_code != SQLITE_ROW)
                is_end = true;
        }
    }

    Query::Iterator &Query::Iterator::operator++() {
        if (sqlite3_step(query->prepared_statement) != SQLITE_ROW)
            is_end = true;
        return *this;
    }

    bool Query::Iterator::operator!=(const Iterator &other) const {
        return end != other.is_end;
    }

    Query::Iterator Query::begin() {
        sqlite3_reset(prepared_statement);
        return Iterator(this, false);
    }

    Query::Iterator Query::end() {
        return Iterator(this, true);
    }
}
