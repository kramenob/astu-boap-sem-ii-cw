#pragma once

#include <string>
#include <unordered_map>

class TemplateEngine {
public:
    static std::string replacePlaceholders(
        const std::string& text,
        const std::unordered_map<std::string, std::string>& values
    );
};
