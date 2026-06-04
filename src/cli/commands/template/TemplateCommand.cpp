#include "TemplateCommand.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cstdlib>

namespace fs = std::filesystem;

static const std::string TEMPLATE_DIR = "src/resources/templates";

static std::string findTemplateFile(const std::string& name)
{
    for (const auto& entry : fs::directory_iterator(TEMPLATE_DIR)) {
        if (!entry.is_regular_file()) continue;

        auto filename = entry.path().stem().string();
        if (filename == name) {
            return entry.path().string();
        }
    }

    return "";
}

int TemplateCommand::execute(const CommandContext& ctx)
{
    auto listIt = ctx.args.find("--list");
    auto showIt = ctx.args.find("--show");
    auto addIt  = ctx.args.find("--add");
    auto rmIt   = ctx.args.find("--remove");

    // ---------------- LIST ----------------
    if (listIt != ctx.args.end()) {
        std::cout << "Templates:\n";

        for (const auto& entry : fs::directory_iterator(TEMPLATE_DIR)) {
            if (!entry.is_regular_file()) continue;
            std::cout << "  - " << entry.path().stem().string() << "\n";
        }

        return 0;
    }

    // ---------------- SHOW ----------------
    if (showIt != ctx.args.end()) {
        const std::string& name = showIt->second;

        std::string file = findTemplateFile(name);

        if (file.empty()) {
            std::cerr << "Template not found: " << name << "\n";
            return 1;
        }

        std::cout << "File will be opened in default program: " << file << "\n";

        std::string cmd;

#ifdef _WIN32
        cmd = "start \"\" \"" + file + "\"";
#elif __APPLE__
        cmd = "open \"" + file + "\"";
#else
        cmd = "xdg-open \"" + file + "\"";
#endif

        system(cmd.c_str());

        return 0;
    }

    // ---------------- ADD ----------------
    if (addIt != ctx.args.end()) {
        const std::string& src = addIt->second;

        auto asIt = ctx.args.find("-as");
        if (asIt == ctx.args.end()) {
            asIt = ctx.args.find("--as");
        }

        if (asIt == ctx.args.end()) {
            std::cerr << "Usage: template --add <file> -as|--as <name>\n";
            return 1;
        }

        std::string name = asIt->second;

        fs::path srcPath(src);

        if (!fs::exists(srcPath)) {
            std::cerr << "Source file not found\n";
            return 1;
        }

        std::string ext = srcPath.extension().string();

        fs::path dst = fs::path(TEMPLATE_DIR) / (name + ext);

        try {
            fs::copy_file(srcPath, dst, fs::copy_options::overwrite_existing);
        } catch (...) {
            std::cerr << "Failed to copy template\n";
            return 1;
        }

        std::cout << "Template added: " << name << "\n";
        return 0;
    }

    // ---------------- REMOVE ----------------
    if (rmIt != ctx.args.end()) {
        const std::string& name = rmIt->second;

        std::string file = findTemplateFile(name);

        if (file.empty()) {
            std::cerr << "Template not found: " << name << "\n";
            return 1;
        }

        try {
            fs::remove(file);
        } catch (...) {
            std::cerr << "Failed to remove template\n";
            return 1;
        }

        std::cout << "Template removed: " << name << "\n";
        return 0;
    }

    std::cerr << "Usage:\n";
    std::cerr << "  template --list\n";
    std::cerr << "  template --show <name>\n";
    std::cerr << "  template --add <file> -as <name>\n";
    std::cerr << "  template --remove <name>\n";

    return 1;
}
