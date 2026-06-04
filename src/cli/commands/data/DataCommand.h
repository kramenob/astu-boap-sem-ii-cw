#pragma once
#include "../../Command.h"

class DataCommand : public Command {
public:
    int execute(const CommandContext& ctx) override;
};
