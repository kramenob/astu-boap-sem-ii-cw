/**
 * @file TemplateCommand.cpp
 * @brief Implementation of CLI template management command.
 *
 * Provides functionality for listing, adding, removing,
 * and viewing template files stored in the user application directory.
 */

/**
 * Project Module Imports
 *
 * Command definition and application metadata constants.
 */
#include "TemplateCommand.h"
#include "../../../core/About.h"

/**
 * Standard Library Imports
 *
 * Console I/O, filesystem operations, file streaming,
 * environment variables, and container utilities.
 */
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <unordered_set>

namespace fs = std::filesystem;

/**
 * @brief Resolves the user-specific application data directory.
 *
 * Uses OS environment variables (HOME/USERPROFILE) and maps to:
 * Documents/<AppName>
 *
 * @return Base application directory path.
 */
static std::filesystem::path getUserDataDir()
{
#ifdef _WIN32
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif

    std::filesystem::path base = home ? std::filesystem::path(home) : std::filesystem::current_path();
    base /= "Documents";
    base /= NAME;
    return base;
}

/**
 * @brief Resolves and ensures the template storage directory exists.
 *
 * Creates the directory if it does not already exist.
 *
 * @return Path to template directory.
 */
static fs::path getTemplateDir()
{
    fs::path dir = getUserDataDir() / "templates";
    fs::create_directories(dir);
    return dir;
}

/**
 * @brief Searches for a template file by its name (stem match).
 *
 * Iterates through available template files and returns
 * the first match based on filename without extension.
 *
 * @param name Template name to search for.
 * @return Full file path if found, otherwise empty string.
 */
static std::string findTemplateFile(const std::string& name)
{
    for (const auto& entry : fs::directory_iterator(getTemplateDir())) {
        if (!entry.is_regular_file()) continue;

        auto filename = entry.path().stem().string();
        if (filename == name) {
            return entry.path().string();
        }
    }

    return "";
}

/**
 * @brief Executes template management CLI operations.
 *
 * Supported operations:
 * - --list   : List all available templates
 * - --show   : Open template file in default system application
 * - --add    : Copy external file into template storage
 * - --remove : Delete template by name
 *
 * Performs strict validation of CLI arguments and options.
 *
 * @param ctx Parsed command-line context.
 * @return Exit code (0 on success, 1 on error).
 */
int TemplateCommand::execute(const CommandContext& ctx)
{
    const std::unordered_set<std::string> allowedOptions = {
        "--list",
        "--show",
        "--add",
        "--remove",
        "-as",
        "--as",
        "--help",
    };

    if (ctx.args.count("--help")) {
        std::cout
            << "Usage: " << NAME << " template [options]\n"
            << "\n"
            << "Options:\n"
            << "  --list\n"
            << "  --show <name>\n"
            << "  --add <file> -as <name>\n"
            << "  --remove <name>\n"
            << "  --help\n";
        return 0;
    }

    for (const auto& [key, value] : ctx.args) {
        if (key.rfind("--", 0) == 0 && !allowedOptions.count(key)) {
            std::cerr << "Error: unknown option '" << key << "'\n";
            std::cerr << "Use: " << NAME << " template --help\n";
            return 1;
        }
    }

    // Strict positional argument validation
    for (const auto& [key, value] : ctx.args) {
        if (key.rfind("--", 0) != 0 && key.rfind("-", 0) != 0) {
            std::cerr << "Error: unknown command or argument '" << key << "'\n";
            std::cerr << "Use: " << NAME << " template --help\n";
            return 1;
        }
    }

    auto listIt = ctx.args.find("--list");
    auto showIt = ctx.args.find("--show");
    auto addIt  = ctx.args.find("--add");
    auto rmIt   = ctx.args.find("--remove");

    // ---------------- LIST ----------------
    if (listIt != ctx.args.end()) {
        std::cout << "Templates:\n";

        for (const auto& entry : fs::directory_iterator(getTemplateDir())) {
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
            std::cerr << "Error: template not found: " << name << "\n";
            std::cerr << "Use: " << NAME << " template --help\n";
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
            std::cerr << "Error: missing required option -as|--as\n";
            std::cerr << "Use: " << NAME << " template --help\n";
            return 1;
        }

        std::string name = asIt->second;

        fs::path srcPath(src);

        if (!fs::exists(srcPath)) {
            std::cerr << "Error: source file not found\n";
            std::cerr << "Use: " << NAME << " template --help\n";
            return 1;
        }

        std::string ext = srcPath.extension().string();

        fs::path dst = getTemplateDir() / (name + ext);

        try {
            fs::copy_file(srcPath, dst, fs::copy_options::overwrite_existing);
        } catch (...) {
            std::cerr << "Error: failed to copy template\n";
            std::cerr << "Use: " << NAME << " template --help\n";
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
            std::cerr << "Error: template not found: " << name << "\n";
            std::cerr << "Use: " << NAME << " template --help\n";
            return 1;
        }

        try {
            fs::remove(file);
        } catch (...) {
            std::cerr << "Error: failed to remove template\n";
            std::cerr << "Use: " << NAME << " template --help\n";
            return 1;
        }

        std::cout << "Template removed: " << name << "\n";
        return 0;
    }

    std::cerr << "Error: no valid template command provided\n";
    std::cerr << "Use: " << NAME << " template --help\n";
    return 1;
}
