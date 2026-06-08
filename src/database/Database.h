/**
 * @file Database.h
 * @brief Database abstraction for SQLite-backed storage.
 *
 * Declares the Database class responsible for
 * schema initialization, table management,
 * data retrieval, and database maintenance.
 */
#pragma once

/**
 * Standard Library Imports
 *
 * String handling and dynamic container support.
 */
#include <string>
#include <vector>

/**
 * @class Database
 * @brief High-level interface for SQLite database operations.
 *
 * Encapsulates database initialization, schema loading,
 * table inspection, data retrieval, and maintenance tasks.
 */
class Database {
public:
    /**
     * @name Constructors
     * Database initialization and connection management.
     */

    /**
     * @brief Opens or creates a database at the specified path.
     */
    Database(const std::string& dbPath);
    /**
     * @brief Opens or creates the default application database.
     */
    Database();

    /**
     * @brief Initializes database structures from a template.
     */
    void init(const std::string& templateName);

    /**
     * @brief Returns all tables available in the database.
     */
    std::vector<std::string> listTables();
    /**
     * @brief Returns user-defined tables excluding system tables.
     */
	std::vector<std::string> listUserTables();
    /**
     * @brief Returns column names for the specified table.
     */
    std::vector<std::string> getColumns(
        const std::string& tableName
    );
    /**
     * @brief Retrieves table rows.
     */
    std::vector<std::vector<std::string>> getRows(
        const std::string& tableName,
        int limit = 5
    );
    /**
     * @brief Removes a table from the database.
     */
    bool dropTable(const std::string& tableName);
    /**
     * @brief Removes all user-accessible tables.
     */
    bool dropAllTables();
    /**
     * @brief Initializes the database from a schema file.
     */
    bool initFromSchema(const std::string& schemaPath);
    /**
     * @brief Returns a formatted schema description.
     */
    std::string showSchema(const std::string& tableName);

    /**
     * @brief Resolves a template name into a filesystem path.
     */
	std::string resolveTemplatePath(const std::string& templateName);
    /**
     * @brief Executes SQL commands from a file.
     */
	bool executeSqlFile(const std::string& path);

private:
    /**
     * Internal database state.
     */
    std::string dbPath;
    void* db;
};