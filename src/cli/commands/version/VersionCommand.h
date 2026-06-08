/**
 * @file VersionCommand.h
 * @brief CLI command for displaying application version information.
 */
#pragma once

/**
 * Project Module Imports
 */
#include "../../Command.h"

/**
 * @class VersionCommand
 * @brief Displays application version and metadata.
 */
class VersionCommand : public Command {
public:
    /**
     * @brief Executes the version command logic.
     *
     * @param ctx Parsed command-line context.
     * @return Exit code of the command execution.
     */
    int execute(const CommandContext& ctx) override;
};
