#pragma once

#include "../../Command.h"

class TemplateCommand : public Command {
public:
    int execute(const CommandContext& ctx) override;
};
