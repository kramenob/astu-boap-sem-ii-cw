/**
 * @file CommandRouter.cpp
 * @brief Implementation of CLI command routing logic.
 */
/**
 * Project Module Imports
 */
#include <iostream>
#include "CommandRouter.h"

/**
 * @brief Routes command execution to the registered handler.
 */
int CommandRouter::dispatch(const std::string& name, const CommandContext& ctx)
{
    if (!commands.count(name)) {
		std::cerr << "Unknown command: " << name << "\n";
		std::cerr << "Try: markforge --help\n";
        return 1;
    }

    return commands[name]->execute(ctx);
}

/**
 * @brief Registers a new command in the router.
 */
void CommandRouter::registerCommand(
    const std::string& name,
    std::unique_ptr<Command> cmd)
{

    commands[name] = std::move(cmd);
}
