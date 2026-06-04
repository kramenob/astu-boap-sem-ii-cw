#include "GenerateCommand.h"
#include "../database/Database.h"
#include "../templates/TemplateEngine.h"
#include <iostream>
#include <fstream>
#include <filesystem>

int GenerateCommand::execute(const CommandContext& ctx)
{
    if (!ctx.args.count("--id") || !ctx.args.count("--template")) {
        std::cerr << "Missing --id or --template\n";
        return 1;
    }

    int id = std::stoi(ctx.args.at("--id"));
    std::string tpl = ctx.args.at("--template");

    Database db;
    TemplateEngine engine;

    Person p = db.getPersonById(id);

    std::unordered_map<std::string, std::string> values = {
        {"FIRST_NAME", p.firstName},
        {"LAST_NAME", p.lastName},
        {"PATRONYMIC", p.patronymic},
        {"AGE", std::to_string(p.age)},
        {"GENDER", p.gender},
        {"CITY", p.city},
        {"ADDRESS", p.address},
        {"EMAIL", p.email},
        {"PHONE", p.phone},
        {"POSITION", p.position}
    };

    std::string result = engine.render(tpl, values);

    std::filesystem::path outDir =
        std::filesystem::path(std::getenv("HOME")) /
        "Documents/boap-sem-ii-cw/output";

    std::filesystem::create_directories(outDir);

    std::ofstream file(
        outDir /
        (std::to_string(p.id) + "_" + p.lastName + ".txt")
    );

    file << result;

    std::cout << "Generated\n";
    return 0;
}