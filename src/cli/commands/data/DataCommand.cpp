// Implementation for DataCommand
#include "DataCommand.h"
#include "../../../database/Database.h"
#include "../../../core/About.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unordered_set>

// Helper function to calculate the number of UTF-8 codepoints in a string
static size_t utf8Length(const std::string& s)
{
    size_t count = 0;
    for (unsigned char c : s) {
        if ((c & 0xC0) != 0x80) {
            ++count;
        }
    }
    return count;
}

int DataCommand::execute(const CommandContext& ctx)
{
    Database db;

    const std::unordered_set<std::string> allowedOptions = {
        "--show",
        "--help"
    };

    if (ctx.args.count("--help")) {
        std::cout
            << "Usage: " << NAME << " data --show [table_name]\n"
            << "\n"
            << "Options:\n"
            << "  --show [table_name]\n"
            << "  --help\n";
        return 0;
    }

    auto showIt = ctx.args.find("--show");

    for (const auto& [key, value] : ctx.args) {
        if (key.rfind("--", 0) == 0 && !allowedOptions.count(key)) {
            std::cerr << "Error: unknown option '" << key << "'\n";
            std::cerr << "Use: " << NAME << " data --help\n";
            return 1;
        }
    }

    if (showIt == ctx.args.end()) {
        std::cerr << "Error: missing required option --show\n";
        std::cerr << "Use: " << NAME << " data --help\n";
        return 1;
    }

    std::string tableName;

    if (showIt->second == "true") {
        auto tables = db.listUserTables();

        if (tables.empty()) {
            std::cout << "No tables found.\n";
            return 0;
        }

        std::cout << "Available tables:\n";

        for (size_t i = 0; i < tables.size(); ++i) {
            std::cout << "  "
                      << (i + 1)
                      << ". "
                      << tables[i]
                      << "\n";
        }

        std::cout << "Choose table to show data [1-"
                  << tables.size()
                  << "]: ";

        size_t choice = 0;
        std::cin >> choice;

        if (choice < 1 || choice > tables.size()) {
            std::cerr << "Error: invalid choice\n";
            std::cerr << "Use: " << NAME << " data --help\n";
            return 1;
        }

        tableName = tables[choice - 1];
    }
    else {
        tableName = showIt->second;
    }

    // Strict positional argument validation
    for (const auto& [key, value] : ctx.args) {
        if (key.rfind("--", 0) != 0 && key != "--show") {
            std::cerr << "Error: unknown command or argument '" << key << "'\n";
            std::cerr << "Use: " << NAME << " data --help\n";
            return 1;
        }
    }

    if (tableName.empty()) {
        std::cerr << "Error: missing table name\n";
        std::cerr << "Use: " << NAME << " data --help\n";
        return 1;
    }

    auto rows = db.getRows(tableName, 5);
    auto columns = db.getColumns(tableName);

    std::cout << "# `"
              << tableName
              << "` DATA (limit 5)\n\n";

    if (rows.empty()) {
        std::cout << "No records found.\n";
        return 0;
    }

    std::vector<size_t> widths(columns.size(), 3);

    for (size_t i = 0; i < columns.size(); ++i) {
        widths[i] = std::max<size_t>(3, utf8Length(columns[i]));
    }

    for (const auto& row : rows) {
        for (size_t i = 0; i < row.size(); ++i) {
            widths[i] = std::max<size_t>(widths[i], std::max<size_t>(3, utf8Length(row[i])));
        }
    }

    // Header
    std::cout << '|';
    for (size_t i = 0; i < columns.size(); ++i) {
        const std::string& cell = columns[i];
        std::cout << ' ' << cell;

        size_t pad = widths[i] > utf8Length(cell)
            ? widths[i] - utf8Length(cell)
            : 0;

        std::cout << std::string(pad, ' ') << " |";
    }
    std::cout << '\n';

    // Separator
    std::cout << '|';
    for (size_t i = 0; i < columns.size(); ++i) {
        std::cout << ' ' << std::string(widths[i], '-') << " |";
    }
    std::cout << '\n';

    // Rows
    for (const auto& row : rows) {
        std::cout << '|';

        for (size_t i = 0; i < row.size(); ++i) {
            const std::string& cell = row[i];

            std::cout << ' ' << cell;

            size_t pad = widths[i] > utf8Length(cell)
                ? widths[i] - utf8Length(cell)
                : 0;

            std::cout << std::string(pad, ' ') << " |";
        }

        std::cout << '\n';
    }

    // Ellipsis row
    std::cout << '|';
    for (size_t i = 0; i < columns.size(); ++i) {
        std::cout << ' ';
        std::cout << "...";

        size_t pad = widths[i] > 3 ? widths[i] - 3 : 0;
        std::cout << std::string(pad, ' ') << " |";
    }
    std::cout << '\n';

    return 0;
}
