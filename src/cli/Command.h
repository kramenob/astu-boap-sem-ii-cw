/**
 * @file Command.h
 * @brief Core command abstraction and CLI context definition.
 *
 * Provides the Command interface and CommandContext structure
 * used for routing and executing CLI commands.
 */
#pragma once
/**
 * Standard Library Imports
 */
#include <string>
#include <unordered_map>

/**
 * @struct CommandContext
 * @brief Runtime context passed to CLI commands.
 *
 * Stores parsed command-line arguments as key-value pairs.
 */
struct CommandContext {
    /**
     * Parsed command-line arguments.
     * Key-value pairs representing CLI flags and parameters.
     */
    std::unordered_map<std::string, std::string> args;
};

/**
 * @class Command
 * @brief Abstract base class for all CLI commands.
 *
 * Defines the interface that all concrete commands must implement.
 */
class Command {
public:
    virtual ~Command() = default;
    /**
     * @brief Executes the command logic.
     *
     * @param ctx Parsed command-line context.
     * @return Exit code of the command execution.
     */
    virtual int execute(const CommandContext& ctx) = 0;
};
