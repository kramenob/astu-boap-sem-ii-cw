#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Command.h"

class CommandRouter {
public:
    void registerCommand(const std::string& name, std::unique_ptr<Command> cmd);

    int dispatch(const std::string& name, const CommandContext& ctx);

private:
    std::unordered_map<std::string, std::unique_ptr<Command>> commands;
};
