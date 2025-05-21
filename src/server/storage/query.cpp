#include "query.h"

#include <utility>
#include "column_cache.h"

namespace Sidequest::Server {

    Query::Query(Database *database, const std::string& statement_sql)
        : database(database), prepared_statement(prepare(statement_sql)) {
    }

    Query::~Query() {
        reset_statement();
        //TODO: FIGMA where finalize?
        //sqlite3_finalize(prepared_statement);
    }

    PreparedStatement *Query::prepare(const std::string &statement_sql) const {
        PreparedStatement *prepared_statement = database->statement_cache->get_statement(statement_sql);
        if (prepared_statement == nullptr)
            prepared_statement = database->statement_cache->add_statement(statement_sql);
        return prepared_statement;
    }

    void Query::execute() {
        this->status_code = sqlite3_step(prepared_statement);
        if (!has_row() && !is_done())
            throw ParameterBindException("error executing query", this->status_code);
    }

    void Query::bind(const int parameter_index, const std::string& value) {
        this->status_code = sqlite3_bind_text(prepared_statement, parameter_index, value.c_str(), -1, SQLITE_TRANSIENT);
        if (!is_ok())
            throw ParameterBindException("error binding parameter " + std::to_string(parameter_index) + " to " + value,
                                         status_code);
    }

    void Query::bind(const int parameter_index, const unsigned int value) {
        this->status_code = sqlite3_bind_int(prepared_statement, parameter_index, value);
        if (!is_ok()) {
            throw ParameterBindException(
                "error binding parameter " + std::to_string(parameter_index) + " to " + std::to_string(value), status_code);
        }
    }

    //TODO: allow other datatype binding
    //TODO: allow other datatype dreading

    void Query::reset_statement() const {
        sqlite3_reset(prepared_statement);
    }

    int Query::read_int_value(const std::string &column_name) const {
        const int column_index = database->column_cache->get_column_index(prepared_statement, column_name);
        const int col_value = static_cast<int>(sqlite3_column_int64(prepared_statement, column_index));
        return col_value;
    }

    std::string Query::read_text_value(const std::string &column_name) const {
        const int column_index = database->column_cache->get_column_index(prepared_statement, column_name);
        const auto col_value = reinterpret_cast<const char *>(sqlite3_column_text(prepared_statement, column_index));
        std::string result(col_value);
        return result;
    }

    bool Query::has_row() const {
        return this->status_code == SQLITE_ROW;
    }

    bool Query::is_done() const {
        return this->status_code == SQLITE_DONE;
    }

    bool Query::is_ok() const {
        return this->status_code == SQLITE_OK;
    }


    //Iterator
    Query::Iterator::Iterator(Query *query, bool is_end) : query(query), is_end(is_end) {
        if (!is_end) {
            query->status_code = sqlite3_step(query->prepared_statement);
            if (query->status_code != SQLITE_ROW)
                is_end = true;
        }
    }

    Query::Iterator &Query::Iterator::operator++() {
        if (sqlite3_step(query->prepared_statement) != SQLITE_ROW)
            is_end = true;
        return *this;
    }

    bool Query::Iterator::operator!=(const Iterator &other) const {
        return is_end != other.is_end;
    }

    Query::Iterator Query::begin() {
        sqlite3_reset(prepared_statement);
        return Iterator(this, false);
    }

    Query::Iterator Query::end() {
        return Iterator(this, true);
    }
}
