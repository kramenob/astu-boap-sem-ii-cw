/**
 * @file DbCommand.h
 * @brief CLI command for database management operations.
 *
 * Provides commands for initializing, inspecting,
 * and modifying the underlying SQLite database.
 */
#pragma once

/**
 * Project Module Imports
 */
#include "../../Command.h"

/**
 * @class DbCommand
 * @brief Handles database-related CLI operations.
 */
class DbCommand : public Command {
public:
    /**
     * @brief Executes the database command logic.
     *
     * @param ctx Parsed command-line context.
     * @return Exit code of the command execution.
     */
    int execute(const CommandContext& ctx) override;
};
