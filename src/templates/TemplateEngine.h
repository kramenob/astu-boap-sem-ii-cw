#pragma once

#include <string>
#include <unordered_map>

class TemplateEngine {
public:
    std::string render(
        const std::string& templateName,
        const std::unordered_map<std::string, std::string>& values
    );
};
