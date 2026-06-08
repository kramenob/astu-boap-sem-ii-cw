/**
 * @file DataCommand.h
 * @brief CLI command for data inspection and manipulation.
 *
 * Provides functionality for viewing, querying,
 * and managing application data through the CLI.
 */
#pragma once

/**
 * Project Module Imports
 */
#include "../../Command.h"

/**
 * @class DataCommand
 * @brief Handles data-related CLI operations.
 */
class DataCommand : public Command {
public:
    /**
     * @brief Executes the data command logic.
     *
     * @param ctx Parsed command-line context.
     * @return Exit code of the command execution.
     */
    int execute(const CommandContext& ctx) override;
};
