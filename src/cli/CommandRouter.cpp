#include <iostream>
#include "CommandRouter.h"

int CommandRouter::dispatch(const std::string& name, const CommandContext& ctx)
{
    if (!commands.count(name)) {
		std::cerr << "Unknown command: " << name << "\n";
		std::cerr << "Try: markforge help\n";
        return 1;
    }

    return commands[name]->execute(ctx);
}

void CommandRouter::registerCommand(
    const std::string& name,
    std::unique_ptr<Command> cmd)
{

    commands[name] = std::move(cmd);
}
