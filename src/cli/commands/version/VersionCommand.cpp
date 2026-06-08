/**
 * @file VersionCommand.cpp
 * @brief Implementation of CLI version display command.
 *
 * Outputs application name, version, description,
 * and copyright/license information.
 */

/**
 * Project Module Imports
 */
#include "VersionCommand.h"
#include "../../../core/About.h"

/**
 * Standard Library Imports
 */
#include <iostream>

/**
 * @brief Displays application version and metadata.
 *
 * Prints NAME, VERSION, DESCRIPTION, and COPYRIGHT.
 *
 * @param ctx Unused command context.
 * @return Exit code (always 0).
 */
int VersionCommand::execute(const CommandContext& ctx)
{
    (void)ctx;

    std::cout << NAME << " " << VERSION << std::endl << DESCRIPTION << std::endl << COPYRIGHT << std::endl;
    return 0;
}
