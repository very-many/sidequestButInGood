#include "query.h"

#include <iostream>
#include <optional>
#include <bits/locale_facets_nonio.h>

#include "column_cache.h"
#include "database.h"

namespace Sidequest::Server {
    Query::Query(Database *database, const std::string &statement_sql) : database(database) {
        prepared_statement = database->statement_cache->get_statement(statement_sql);
        if (prepared_statement == nullptr)
            prepared_statement = database->statement_cache->add_statement(statement_sql);
    }

    Query::~Query() {
        sqlite3_reset(prepared_statement);
    }

    void Query::execute() {
        this->status_code = sqlite3_step(prepared_statement);
    }

    void Query::bind(const int parameter_index, const std::string &value) {
        this->status_code = sqlite3_bind_text(prepared_statement, parameter_index, value.c_str(), -1, SQLITE_TRANSIENT);
        if (!is_ok())
            throw ParameterBindException("error binding parameter " + std::to_string(parameter_index) + " to " + value,
                                         status_code);
    }

    void Query::bind(const int parameter_index, const long value) {
        this->status_code = sqlite3_bind_int64(prepared_statement, parameter_index, value);
        if (!is_ok()) {
            throw ParameterBindException(
                "error binding parameter " + std::to_string(parameter_index) + " to " + std::to_string(value),
                status_code);
        }
    }

    void Query::bind(int parameter_index, const std::optional<long>& value) {
        if (value.has_value()) {
            bind(parameter_index, value.value());
        } else {
            bind_null(parameter_index);
        }
    }

    void Query::bind_null(const int parameter_index) {
        this->status_code = sqlite3_bind_null(prepared_statement, parameter_index);
        if (!is_ok())
            throw ParameterBindException("error binding parameter" + std::to_string(parameter_index) + "to null",
                status_code);
    }

    long Query::read_integer_value(const std::string &column_name) const {
        const int column_index = database->column_cache->get_column_index(prepared_statement, column_name);
        const auto col_value = sqlite3_column_int64(prepared_statement, column_index);
        return static_cast<long>(col_value);
    }

    std::optional<long> Query::read_optional_integer_value(const std::string &column_name) const {
        const int column_index = database->column_cache->get_column_index(prepared_statement, column_name);
        if (sqlite3_column_type(prepared_statement, column_index) == SQLITE_NULL)
            return std::nullopt;
        return sqlite3_column_int64(prepared_statement, column_index);
    }

    std::string Query::read_text_value(const std::string &column_name) const {
        const int column_index = database->column_cache->get_column_index(prepared_statement, column_name);
        const auto col_value = reinterpret_cast<const char *>(sqlite3_column_text(prepared_statement, column_index));
        if (col_value)
            return std::string{col_value};
        return "";
    }

    bool Query::has_row() const {
        return this->status_code == SQLITE_ROW;
    }

    bool Query::is_done() const {
        return this->status_code == SQLITE_DONE;
    }

    Quest::Id Query::last_row_id() const {
        return sqlite3_last_insert_rowid(database->getHandle());
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
        return Iterator(this, false);
    }

    Query::Iterator Query::end() {
        return Iterator(this, true);
    }
}
