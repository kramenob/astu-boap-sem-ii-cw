#pragma once
#include "../../Command.h"

class DbCommand : public Command {
public:
    int execute(const CommandContext& ctx) override;
};
