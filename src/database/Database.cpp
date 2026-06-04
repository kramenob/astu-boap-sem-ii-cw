#include "Database.h"
#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

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
    std::filesystem::path base;

#ifdef _WIN32
    base = std::getenv("USERPROFILE");
#else
    base = std::getenv("HOME");
#endif

    base /= "Documents/boap-sem-ii-cw/database";

    std::filesystem::create_directories(base);

    dbPath = (base / "persons.db").string();

    sqlite3_open(dbPath.c_str(), (sqlite3**)&db);
}

void Database::init() {

	// Debug: Clear the table before inserting new data
	sqlite3_exec((sqlite3*)db, "DELETE FROM persons;", nullptr, nullptr, nullptr);

    const char* sql =
        "CREATE TABLE IF NOT EXISTS persons ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"first_name TEXT,"
			"last_name TEXT,"
			"patronymic TEXT,"
			"age INTEGER,"
			"gender TEXT,"
			"city TEXT,"
			"address TEXT,"
			"email TEXT,"
			"phone TEXT,"
			"position TEXT,"
			"created_at INTEGER,"
			"updated_at INTEGER"
		");"

		"INSERT OR REPLACE INTO persons VALUES "
		"(1,'Иван','Иванов','Иванович',25,'Male','Москва','Красная площадь','ivan@example.com','+79990000001','Инженер',strftime('%s','now'),strftime('%s','now'));"

		"INSERT OR REPLACE INTO persons VALUES "
		"(2,'Пётр','Петров','Петрович',31,'Male','Санкт-Петербург','Невский проспект','petr@example.com','+79990000002','Менеджер',strftime('%s','now'),strftime('%s','now'));"

		"INSERT OR REPLACE INTO persons VALUES "
		"(3,'Алексей','Сидоров','Алексеевич',28,'Male','Казань','ул. Баумана','alex@example.com','+79990000003','Аналитик',strftime('%s','now'),strftime('%s','now'));"

		"INSERT OR REPLACE INTO persons VALUES "
		"(4,'Мария','Кузнецова','Ивановна',27,'Female','Минск','проспект Независимости','maria@example.com','+375290000004','Дизайнер',strftime('%s','now'),strftime('%s','now'));"

		"INSERT OR REPLACE INTO persons VALUES "
		"(5,'Дмитрий','Смирнов','Дмитриевич',35,'Male','Киев','Крещатик','dmitry@example.com','+380990000005','Архитектор',strftime('%s','now'),strftime('%s','now'));";

    char* err = nullptr;

    sqlite3_exec((sqlite3*)db, sql, nullptr, nullptr, &err);

    if (err) {
        std::cerr << "SQL error: " << err << "\n";
        sqlite3_free(err);
    }
}

void Database::touchUpdatedAt(int id) {
	std::string sql = "UPDATE persons SET updated_at = strftime('%s','now') WHERE id = " + std::to_string(id) + ";";
	char* err = nullptr;
	sqlite3_exec((sqlite3*)db, sql.c_str(), nullptr, nullptr, &err);
	if (err) {
		std::cerr << "SQL error: " << err << "\n";
		sqlite3_free(err);
	}
}

Person Database::getPersonById(int id) {
    Person p{};
    p.id = -1;

    std::string sql =
        "SELECT id, first_name, last_name, patronymic, age, gender, "
        "city, address, email, phone, position, created_at, updated_at "
        "FROM persons WHERE id = " + std::to_string(id) + ";";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2((sqlite3*)db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement\n";
        return p;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {

        p.id = sqlite3_column_int(stmt, 0);

        p.firstName   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        p.lastName    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        p.patronymic  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        p.age         = sqlite3_column_int(stmt, 4);
        p.gender      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        p.city        = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        p.address     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));

        p.email       = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        p.phone       = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));

        p.position    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));

        p.createdAt   = sqlite3_column_int64(stmt, 11);
        p.updatedAt   = sqlite3_column_int64(stmt, 12);
    }

    sqlite3_finalize(stmt);
    return p;
}

void Database::reset()
{
    sqlite3_exec((sqlite3*)db, "DROP TABLE IF EXISTS persons;", nullptr, nullptr, nullptr);
    init();
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
    std::string seedFile = resolveTemplatePath(templateName);

    if (!executeSqlFile(seedFile)) {
        std::cerr << "Failed to load template: " << templateName << "\n";
        return;
    }
}

std::string Database::resolveTemplatePath(const std::string& templateName)
{
    return "src/resources/db/seed_" + templateName + ".sql";
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