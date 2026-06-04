#pragma once
#include "Command.h"

class GenerateCommand : public Command {
public:
    int execute(const CommandContext& ctx) override;
};
