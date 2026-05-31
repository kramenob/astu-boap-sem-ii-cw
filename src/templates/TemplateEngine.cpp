#include "TemplateEngine.h"

std::string TemplateEngine::replacePlaceholders(
    const std::string& text,
    const std::unordered_map<std::string, std::string>& values
) {
    std::string result = text;

    for (const auto& [key, value] : values) {
        std::string placeholder = "{{" + key + "}}";

        size_t pos = 0;
        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
            result.replace(pos, placeholder.length(), value);
            pos += value.length();
        }
    }

    return result;
}
