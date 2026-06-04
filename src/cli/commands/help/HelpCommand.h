#pragma once
#include "../../Command.h"

class HelpCommand : public Command {
public:
    int execute(const CommandContext& ctx) override;
};
