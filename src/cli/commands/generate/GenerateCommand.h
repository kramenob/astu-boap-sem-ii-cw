/**
 * @file GenerateCommand.h
 * @brief CLI command for generating output based on templates.
 *
 * Responsible for generating structured output
 * such as documents, schemas, or configuration files.
 */
#pragma once

/**
 * Project Module Imports
 */
#include "../../Command.h"

/**
 * @class GenerateCommand
 * @brief Handles generation of structured output.
 */
class GenerateCommand : public Command {
public:
    /**
     * @brief Executes the generation command logic.
     *
     * @param ctx Parsed command-line context.
     * @return Exit code of the command execution.
     */
    int execute(const CommandContext& ctx) override;
};
