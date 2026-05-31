#pragma once

#include "../models/Person.h"
#include <string>

class Database {
	public:
		Database(const std::string& dbPath);
		~Database();

		void init();
		void touchUpdatedAt(int id);
		Person getPersonById(int id);

	private:
		std::string dbPath;
		void* db; // sqlite3*
};
