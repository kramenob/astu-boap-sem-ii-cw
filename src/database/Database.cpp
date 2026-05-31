#include "Database.h"
#include <iostream>
#include <sqlite3.h>
#include <filesystem>

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

Database::~Database() {
    if (db) {
        sqlite3_close((sqlite3*)db);
    }
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
