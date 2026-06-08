/**
 * @file HelpCommand.h
 * @brief CLI command providing usage help and documentation.
 *
 * Displays available commands, usage instructions,
 * and descriptions of CLI functionality.
 */
#pragma once

/**
 * Project Module Imports
 */
#include "../../Command.h"

/**
 * @class HelpCommand
 * @brief Displays help information for CLI usage.
 */
class HelpCommand : public Command {
public:
    /**
     * @brief Executes the help command logic.
     *
     * @param ctx Parsed command-line context.
     * @return Exit code of the command execution.
     */
    int execute(const CommandContext& ctx) override;
};
