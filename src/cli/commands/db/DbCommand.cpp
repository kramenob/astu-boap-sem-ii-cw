#include "DbCommand.h"
#include "../../../database/Database.h"
#include "../../../core/About.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

int DbCommand::execute(const CommandContext& ctx)
{
    Database db;

    const std::unordered_set<std::string> allowedOptions = {
        "--init",
        "--interactive",
        "--schema",
        "--schema-show",
        "--template",
        "--templates",
        "--drop",
        "--list",
        "--all",
        "--help",
    };

    if (ctx.args.count("--help")) {
        std::cout
            << "Usage: " << NAME << " db [options]\n"
            << "\n"
            << "Options:\n"
            << "  --init\n"
            << "  --interactive\n"
            << "  --schema <file>\n"
            << "  --schema-show <table>\n"
            << "  --template <name>\n"
            << "  --templates\n"
            << "  --drop <table>\n"
            << "  --list\n"
            << "  --all\n"
            << "  --help\n";
        return 0;
    }

    const bool init          = ctx.args.count("--init");
    const bool interactive   = ctx.args.count("--interactive");
    const bool schema        = ctx.args.count("--schema");
    const bool schemaShow    = ctx.args.count("--schema-show");
    const bool templ         = ctx.args.count("--template");
    const bool listTemplates = ctx.args.count("--templates");
    const bool drop          = ctx.args.count("--drop");
    const bool list          = ctx.args.count("--list");
    const bool all           = ctx.args.count("--all");

    for (const auto& [key, value] : ctx.args) {
        if (key.rfind("--", 0) == 0) {
            if (!allowedOptions.count(key)) {
                std::cerr << "Error: unknown option '" << key << "'\n";
                std::cerr << "Use: " << NAME << " db --help\n";
                return 1;
            }
        }
    }

    for (const auto& [key, value] : ctx.args) {
        // Detect positional or unknown command-like arguments (e.g. "absurd")
        if (key.rfind("--", 0) != 0) {
            std::cerr << "Error: unknown command or argument '" << key << "'\n";
            std::cerr << "Use: " << NAME << " db --help\n";
            return 1;
        }
    }

    // markforge db --schema-show students
    if (schemaShow) {
        auto it = ctx.args.find("--schema-show");

        if (it == ctx.args.end() || it->second.empty() || it->second == "true") {
            std::cerr << "Usage: " << NAME << " db --schema-show <table_name>\n";
            return 1;
        }

        std::cout << db.showSchema(it->second) << '\n';
        return 0;
    }

    // markforge db --list
    if (list) {
        auto tables = db.listTables();

		if (!all) {
			std::vector<std::string> filtered;

			for (const auto& t : tables) {
				if (t == "sqlite_sequence") continue;
				filtered.push_back(t);
			}

			tables = filtered;
		}

        if (tables.empty()) {
            std::cout << "No tables found.\n";
            return 0;
        }

        std::cout << "Tables:\n";

        for (const auto& table : tables) {
            std::cout << "  - " << table << '\n';
        }

        return 0;
    }

    // markforge db --drop students
    if (drop) {
        if (all) {
            if (!db.dropAllTables()) {
                std::cerr << "Failed to drop all tables.\n";
                return 1;
            }

            std::cout << "Dropped all tables.\n";
            return 0;
        }

        auto it = ctx.args.find("--drop");

        if (it == ctx.args.end() || it->second.empty() || it->second == "true") {
            std::cerr << "Usage: " << NAME << " db --drop <table_name>\n";
            return 1;
        }

        if (!db.dropTable(it->second)) {
            std::cerr << "Failed to drop table: " << it->second << '\n';
            return 1;
        }

        std::cout << "Dropped table: " << it->second << '\n';
        return 0;
    }

    // markforge db --schema schema.sql
    if (schema) {
        const auto schemaPath = ctx.args.at("--schema");

        if (!db.initFromSchema(schemaPath)) {
            std::cerr << "Failed to load schema: " << schemaPath << '\n';
            return 1;
        }

        std::cout << "Schema initialized.\n";
        return 0;
    }

	// markforge db --templates
	if (listTemplates) {
		std::cout << "Available DB templates:\n"
				<< "  - students\n"
				<< "  - teachers\n";
		return 0;
	}

    // markforge db --init
    if (init) {
        std::string templateName = "students";

        if (templ) {
            templateName = ctx.args.at("--template");
        }

		if (interactive) {
			std::cout
				<< "Select template:\n"
				<< "1. students\n"
				<< "2. teachers\n"
				<< "Enter choice: ";

			int choice = 0;
			std::cin >> choice;

			switch (choice) {
				case 1:
					templateName = "students";
					break;
				case 2:
					templateName = "teachers";
					break;
				default:
					std::cout << "Invalid choice, using default (students)\n";
					templateName = "students";
					break;
			}
		}

        const std::unordered_set<std::string> validTemplates = {
            "students",
            "teachers"
        };

        if (!validTemplates.count(templateName)) {
            std::cerr << "Error: unknown template '" << templateName << "'\n";
            std::cerr << "Use: " << NAME << " db --help\n";
            return 1;
        }

        db.init(templateName);

        std::cout
            << "Database initialized using default template: "
            << templateName
            << '\n';

        return 0;
    }

    std::cerr << "Error: no valid db command provided.\n";
    std::cerr << "Use: " << NAME << " db --help\n";
    return 1;
}