#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <ctime>

#include "database/Database.h"
#include "cli/Command.h"
#include "cli/CommandRouter.h"
#include "cli/commands/help/HelpCommand.h"
#include "cli/commands/generate/GenerateCommand.h"
#include "cli/commands/db/DbCommand.h"
#include "cli/commands/data/DataCommand.h"
#include "cli/commands/template/TemplateCommand.h"
#include "cli/commands/version/VersionCommand.h"

CommandContext parse(int argc, char** argv)
{
    CommandContext ctx;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        // support both --flag and -flag
        if (arg.rfind("-", 0) == 0) {
            if (i + 1 < argc) {
                std::string next = argv[i + 1];

                // next token is value if it is not another flag
                if (next.rfind("-", 0) != 0) {
                    ctx.args[arg] = next;
                    ++i;
                    continue;
                }
            }

            ctx.args[arg] = "true";
        }
    }

    return ctx;
}

int main(int argc, char** argv)
{
    CommandRouter router;

    router.registerCommand("generate", std::make_unique<GenerateCommand>());
    router.registerCommand("db", std::make_unique<DbCommand>());
    router.registerCommand("data", std::make_unique<DataCommand>());
    router.registerCommand("template", std::make_unique<TemplateCommand>());

    router.registerCommand("version",   std::make_unique<VersionCommand>());
    router.registerCommand("--version", std::make_unique<VersionCommand>());
    router.registerCommand("--v",       std::make_unique<VersionCommand>());
    router.registerCommand("--status",  std::make_unique<VersionCommand>());

	router.registerCommand("help", std::make_unique<HelpCommand>());
	router.registerCommand("--help", std::make_unique<HelpCommand>());

    if (argc < 2) {
        return router.dispatch("help", CommandContext{});
    }

    CommandContext ctx = parse(argc, argv);

    return router.dispatch(argv[1], ctx);
}
