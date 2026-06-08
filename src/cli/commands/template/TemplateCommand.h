/**
 * @file TemplateCommand.h
 * @brief CLI command for managing templates.
 *
 * Allows listing, loading, and managing embedded
 * or external templates used by the system.
 */
#pragma once

/**
 * Project Module Imports
 */
#include "../../Command.h"

/**
 * @class TemplateCommand
 * @brief Handles template management operations.
 */
class TemplateCommand : public Command {
public:
    /**
     * @brief Executes the template command logic.
     *
     * @param ctx Parsed command-line context.
     * @return Exit code of the command execution.
     */
    int execute(const CommandContext& ctx) override;
};
