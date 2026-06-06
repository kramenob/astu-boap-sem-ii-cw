#pragma once
#include "../../Command.h"

class VersionCommand : public Command {
public:
    int execute(const CommandContext& ctx) override;
};
