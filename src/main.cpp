#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <ctime>

#include "templates/TemplateEngine.h"
#include "database/Database.h"
#include "models/Person.h"
#include "cli/commands/help/HelpCommand.h"
#include "cli/Command.h"
#include "cli/CommandRouter.h"
#include "cli/GenerateCommand.h"
#include "cli/commands/db/DbCommand.h"
#include "cli/TemplateCommand.h"

CommandContext parse(int argc, char** argv)
{
    CommandContext ctx;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.rfind("--", 0) == 0) {
            if (i + 1 < argc) {
                std::string next = argv[i + 1];

                if (next.rfind("--", 0) != 0) {
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
    router.registerCommand("template", std::make_unique<TemplateCommand>());

	router.registerCommand("help", std::make_unique<HelpCommand>());
	router.registerCommand("--help", std::make_unique<HelpCommand>());

    CommandContext ctx = parse(argc, argv);

    return router.dispatch(argv[1], ctx);
}
