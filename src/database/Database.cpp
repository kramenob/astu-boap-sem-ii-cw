/**
 * @file Database.cpp
 * @brief SQLite database management implementation.
 *
 * Provides database initialization, schema inspection,
 * table management, and data retrieval utilities.
 */

/**
 * Project Module Imports
 */
#include "Database.h"
#include "EmbeddedSql.h"
#include "../core/About.h"

/**
 * Third-Party and Standard Library Imports
 *
 * SQLite integration, filesystem access,
 * file processing, formatting, and algorithms.
 */
#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include <cstdlib>

/**
 * @brief Resolves the user-specific application data directory.
 *
 * @return Path to the application's document storage location.
 */
static std::filesystem::path getUserDataDir()
{
#ifdef _WIN32
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif

    std::filesystem::path base = home ? std::filesystem::path(home) : std::filesystem::current_path();
    base /= "Documents";
    base /= NAME;
    return base;
}

/**
 * @brief Opens or creates a database at the specified path.
 */
Database::Database(const std::string& path) : dbPath(path), db(nullptr) {

    std::filesystem::create_directories(
        std::filesystem::path(dbPath).parent_path()
    );

    if (sqlite3_open(dbPath.c_str(), (sqlite3**)&db) != SQLITE_OK) {
        sqlite3* tmp;
		if (sqlite3_open(dbPath.c_str(), &tmp) != SQLITE_OK) {
			std::cerr << "Cannot open DB: " << sqlite3_errmsg(tmp) << "\n";
			return;
		}
		db = tmp;
    }
}

/**
 * @brief Opens or creates the default application database.
 */
Database::Database()
{
    std::filesystem::path base = getUserDataDir() / "database";

    std::filesystem::create_directories(base);

    dbPath = (base / (std::string(NAME) + ".db")).string();

    sqlite3_open(dbPath.c_str(), (sqlite3**)&db);
}

/**
 * @brief Returns all tables present in the database.
 */
std::vector<std::string> Database::listTables()
{
    std::vector<std::string> tables;

    const char* sql =
        "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2((sqlite3*)db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to list tables\n";
        return tables;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* name = sqlite3_column_text(stmt, 0);
        if (name) {
            tables.emplace_back(reinterpret_cast<const char*>(name));
        }
    }

    sqlite3_finalize(stmt);

    return tables;
}

/**
 * @brief Returns user-defined tables excluding SQLite internals.
 */
std::vector<std::string> Database::listUserTables()
{
    auto tables = listTables();

    tables.erase(
        std::remove(
            tables.begin(),
            tables.end(),
            "sqlite_sequence"
        ),
        tables.end()
    );

    return tables;
}

/**
 * @brief Removes a table from the database.
 */
bool Database::dropTable(const std::string& tableName)
{
    std::string sql =
        "DROP TABLE IF EXISTS " + tableName + ";";

    char* err = nullptr;

    sqlite3_exec(
        (sqlite3*)db,
        sql.c_str(),
        nullptr,
        nullptr,
        &err
    );

    if (err) {
        sqlite3_free(err);
        return false;
    }

    return true;
}

/**
 * @brief Removes all user-accessible tables.
 */
bool Database::dropAllTables()
{
    auto tables = listTables();

    for (const auto& table : tables) {
        if (table == "sqlite_sequence") {
            continue;
        }

        if (!dropTable(table)) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Initializes the database from an external schema.
 */
bool Database::initFromSchema(const std::string& schemaPath)
{
    std::cout
        << "Schema loading not implemented yet: "
        << schemaPath
        << "\n";

    return true;
}

/**
 * @brief Produces a formatted schema description for a table.
 */
std::string Database::showSchema(const std::string& tableName)
{
    std::string sql = "PRAGMA table_info(" + tableName + ");";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2((sqlite3*)db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return "Failed to read schema.";
    }

    struct Column {
        std::string name;
        std::string type;
        bool primaryKey;
    };

    std::vector<Column> columns;
    size_t maxNameLength = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Column col;

        const unsigned char* name = sqlite3_column_text(stmt, 1);
        const unsigned char* type = sqlite3_column_text(stmt, 2);

        col.name = name ? reinterpret_cast<const char*>(name) : "";
        col.type = type ? reinterpret_cast<const char*>(type) : "";
        col.primaryKey = sqlite3_column_int(stmt, 5) != 0;

        maxNameLength = std::max(maxNameLength, col.name.size());
        columns.push_back(col);
    }

    sqlite3_finalize(stmt);

    if (columns.empty()) {
        return "Table not found.";
    }

    std::stringstream out;

    out << tableName << " SCHEMA\n";

    for (const auto& col : columns) {
        out << " - "
            << std::left
            << std::setw(static_cast<int>(maxNameLength))
            << col.name
            << ' '
            << col.type;

        if (col.primaryKey) {
            out << " PRIMARY KEY";
        }

        out << '\n';
    }

    return out.str();
}

/**
 * @brief Creates database structures from an embedded template.
 */
void Database::init(const std::string& templateName)
{
    std::string sql;

    if (templateName == "students") {
        sql = EmbeddedSql::students;
    } else if (templateName == "teachers") {
        sql = EmbeddedSql::teachers;
    } else {
        std::cerr << "Unknown template: " << templateName << "\n";
        return;
    }

    char* err = nullptr;

    int rc = sqlite3_exec(
        (sqlite3*)db,
        sql.c_str(),
        nullptr,
        nullptr,
        &err
    );

    if (rc != SQLITE_OK) {
        std::string message = err ? err : "unknown";
        if (err) sqlite3_free(err);

        if (message.find("already exists") != std::string::npos) {
            return;
        }

        std::cerr << "SQL error: " << message << "\n";
        std::cerr << "Failed to load template: " << templateName << "\n";
        return;
    }
}

/**
 * @brief Resolves a template name to a template path.
 */
std::string Database::resolveTemplatePath(const std::string& templateName)
{
    return {};
}

/**
 * @brief Executes SQL statements from a file.
 */
bool Database::executeSqlFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Cannot open SQL file: " << path << "\n";
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string sql = buffer.str();

    char* err = nullptr;

    int rc = sqlite3_exec(
        (sqlite3*)db,
        sql.c_str(),
        nullptr,
        nullptr,
        &err
    );

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << err << "\n";
        sqlite3_free(err);
        return false;
    }

    return true;
}

/**
 * @brief Returns column names for the specified table.
 */
std::vector<std::string> Database::getColumns(
    const std::string& tableName)
{
    std::vector<std::string> columns;

    std::string sql =
        "PRAGMA table_info(" + tableName + ");";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2((sqlite3*)db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return columns;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* name = sqlite3_column_text(stmt, 1);

        columns.push_back(
            name
                ? reinterpret_cast<const char*>(name)
                : ""
        );
    }

    sqlite3_finalize(stmt);

    return columns;
}

/**
 * @brief Retrieves table rows up to the specified limit.
 */
std::vector<std::vector<std::string>> Database::getRows(
    const std::string& tableName,
    int limit)
{
    std::vector<std::vector<std::string>> rows;

    std::string sql =
        "SELECT * FROM " + tableName +
        " LIMIT " + std::to_string(limit) + ";";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2((sqlite3*)db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return rows;
    }

    int columnCount = sqlite3_column_count(stmt);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;

        for (int i = 0; i < columnCount; ++i) {
            const unsigned char* value = sqlite3_column_text(stmt, i);
            row.push_back(
                value
                    ? reinterpret_cast<const char*>(value)
                    : "NULL"
            );
        }

        rows.push_back(row);
    }

    sqlite3_finalize(stmt);

    return rows;
}