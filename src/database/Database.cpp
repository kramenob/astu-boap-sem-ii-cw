#include "Database.h"
#include "EmbeddedSql.h"
#include "../core/About.h"
#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include <cstdlib>

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

Database::Database()
{
    std::filesystem::path base = getUserDataDir() / "database";

    std::filesystem::create_directories(base);

    dbPath = (base / (std::string(NAME) + ".db")).string();

    sqlite3_open(dbPath.c_str(), (sqlite3**)&db);
}

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

bool Database::initFromSchema(const std::string& schemaPath)
{
    std::cout
        << "Schema loading not implemented yet: "
        << schemaPath
        << "\n";

    return true;
}

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

std::string Database::resolveTemplatePath(const std::string& templateName)
{
    return {};
}

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