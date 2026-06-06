#pragma once

#include <string>
#include <vector>

class Database {
public:
    Database(const std::string& dbPath);
    Database();

    void init(const std::string& templateName);

    std::vector<std::string> listTables();
	std::vector<std::string> listUserTables();
    std::vector<std::string> getColumns(
        const std::string& tableName
    );
    std::vector<std::vector<std::string>> getRows(
        const std::string& tableName,
        int limit = 5
    );
    bool dropTable(const std::string& tableName);
    bool dropAllTables();
    bool initFromSchema(const std::string& schemaPath);
    std::string showSchema(const std::string& tableName);

	std::string resolveTemplatePath(const std::string& templateName);
	bool executeSqlFile(const std::string& path);

private:
    std::string dbPath;
    void* db;
};