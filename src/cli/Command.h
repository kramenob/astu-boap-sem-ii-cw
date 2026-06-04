#pragma once
#include <string>
#include <unordered_map>

struct CommandContext {
    std::unordered_map<std::string, std::string> args;
};

class Command {
public:
    virtual ~Command() = default;
    virtual int execute(const CommandContext& ctx) = 0;
};
