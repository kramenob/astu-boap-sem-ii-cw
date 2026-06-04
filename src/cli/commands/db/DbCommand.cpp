#include "DbCommand.h"
#include "../../../database/Database.h"

#include <iostream>
#include <string>
#include <vector>

int DbCommand::execute(const CommandContext& ctx)
{
    Database db;

    const bool init          = ctx.args.count("--init");
    const bool reset         = ctx.args.count("--reset");
    const bool interactive   = ctx.args.count("--interactive");
    const bool schema        = ctx.args.count("--schema");
    const bool schemaShow    = ctx.args.count("--schema-show");
    const bool templ         = ctx.args.count("--template");
	const bool listTemplates = ctx.args.count("--templates");
    const bool drop          = ctx.args.count("--drop");
    const bool list          = ctx.args.count("--list");
	const bool all           = ctx.args.count("--all");

    if (!init && !reset && !schema && !schemaShow && !drop && !list) {
        std::cerr
            << "Usage:\n"
            << "  markforge db --init\n"
            << "  markforge db --init --interactive\n"
            << "  markforge db --init --template <employees|students|patients>\n"
            << "  markforge db --templates\n"
            << "  markforge db --schema <file.sql>\n"
            << "  markforge db --schema-show <table_name>\n"
            << "  markforge db --reset\n"
            << "  markforge db --drop <table_name>\n"
            << "  markforge db --drop --all\n"
            << "  markforge db --list\n"
            << "  markforge db --list --all\n";

        return 1;
    }
    // markforge db --schema-show persons
    if (schemaShow) {
        auto it = ctx.args.find("--schema-show");

        if (it == ctx.args.end() || it->second.empty() || it->second == "true") {
            std::cerr << "Usage: markforge db --schema-show <table_name>\n";
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

    // markforge db --drop persons
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
            std::cerr << "Usage: markforge db --drop <table_name>\n";
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

    // markforge db --reset
    if (reset) {
        db.reset();
        std::cout << "Database reset.\n";
        return 0;
    }

	// markforge db --templates
	if (listTemplates) {
		std::cout << "Available DB templates:\n"
				<< "  - students\n"
				<< "  - employees\n"
				<< "  - patients\n";
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
				<< "2. employees\n"
				<< "3. patients\n"
				<< "Enter choice: ";

			int choice = 0;
			std::cin >> choice;

			switch (choice) {
				case 1:
					templateName = "students";
					break;
				case 2:
					templateName = "employees";
					break;
				case 3:
					templateName = "patients";
					break;
				default:
					std::cout << "Invalid choice, using default (students)\n";
					templateName = "students";
					break;
			}
		}

        db.init(templateName);

        std::cout
            << "Database initialized using template: "
            << templateName
            << '\n';

        return 0;
    }

    return 0;
}