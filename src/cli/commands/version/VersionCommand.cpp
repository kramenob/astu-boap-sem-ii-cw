#include "VersionCommand.h"
#include "../../../core/About.h"

#include <iostream>

int VersionCommand::execute(const CommandContext& ctx)
{
    (void)ctx;

    std::cout << NAME << " " << VERSION << std::endl << DESCRIPTION << std::endl << COPYRIGHT << std::endl;
    return 0;
}
