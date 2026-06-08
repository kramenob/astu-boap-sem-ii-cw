/**
 * @file CommandRouter.h
 * @brief CLI command registration and dispatch system.
 *
 * Manages command registration and routes execution
 * to the appropriate command implementation.
 */
#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Command.h"

/**
 * @class CommandRouter
 * @brief Dispatches CLI commands to registered handlers.
 *
 * Stores command implementations and executes them
 * based on user input.
 */
class CommandRouter {
public:
    /**
     * @brief Registers a command with a given name.
     *
     * @param name Command identifier.
     * @param cmd Command implementation instance.
     */
    void registerCommand(const std::string& name, std::unique_ptr<Command> cmd);

    /**
     * @brief Dispatches a command by name.
     *
     * @param name Command identifier.
     * @param ctx Execution context.
     * @return Command exit code.
     */
    int dispatch(const std::string& name, const CommandContext& ctx);

private:
    std::unordered_map<std::string, std::unique_ptr<Command>> commands;
};
