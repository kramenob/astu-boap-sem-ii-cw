#include "TemplateEngine.h"
#include <fstream>
#include <sstream>

std::string TemplateEngine::render(
    const std::string& templateName,
    const std::unordered_map<std::string, std::string>& values)
{
    std::ifstream file("templates/" + templateName + ".txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    for (const auto& [key, value] : values) {
        std::string placeholder = "{{" + key + "}}";
        size_t pos = 0;
        while ((pos = content.find(placeholder, pos)) != std::string::npos) {
            content.replace(pos, placeholder.length(), value);
            pos += value.length();
        }
    }
    return content;
}
