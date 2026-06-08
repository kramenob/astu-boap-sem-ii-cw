/**
 * @file GenerateCommand.cpp
 * @brief Implementation of the GenerateCommand CLI operation.
 *
 * Generates output files from database rows using templates.
 * Supports both text-based templates and DOCX template processing.
 */
#include "GenerateCommand.h"

/**
 * Standard Library Imports
 *
 * File I/O, string manipulation, filesystem utilities,
 * date/time processing, and system interaction.
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <cstdlib>

/**
 * Project Module Imports
 *
 * Database access layer and application metadata constants.
 */
#include "../../../database/Database.h"
#include "../../../core/About.h"

namespace fs = std::filesystem;

/**
 * @brief Returns current system date and time as a formatted string.
 *
 * Used for timestamped output directory generation.
 * Format: YYYY-MM-DD_HH-MM-SS
 *
 * @return Formatted date-time string.
 */
static std::string getCurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

/**
 * @brief Loads entire file content into a string.
 *
 * @param path Path to the input file.
 * @return File content as a string, or empty string on failure.
 */
static std::string loadFile(const std::string& path)
{
    std::ifstream in(path);
    if (!in.is_open()) return "";

    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

/**
 * @brief Resolves user application data directory.
 *
 * Typically points to ~/Documents/<AppName>.
 *
 * @return Filesystem path to user data directory.
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
 * @brief Resolves and ensures the template directory exists.
 *
 * @return Path to template storage directory.
 */
static std::filesystem::path getTemplateDir()
{
    std::filesystem::path dir = getUserDataDir() / "templates";
    std::filesystem::create_directories(dir);
    return dir;
}

/**
 * @brief Searches for a template file by name (without extension).
 *
 * Iterates through the template directory and matches file stem.
 *
 * @param name Template name to search for.
 * @return Full file path if found, otherwise empty string.
 */
static std::string findTemplateFile(const std::string& name)
{
    for (const auto& entry : std::filesystem::directory_iterator(getTemplateDir())) {
        if (!entry.is_regular_file()) continue;

        if (entry.path().stem().string() == name) {
            return entry.path().string();
        }
    }

    return "";
}

/**
 * @brief Replaces all occurrences of a substring with a value.
 *
 * @param str Input string.
 * @param key Placeholder key to replace.
 * @param value Replacement value.
 * @return Modified string with replacements applied.
 */
static std::string replaceAll(std::string str,
                              const std::string& key,
                              const std::string& value)
{
    size_t pos = 0;
    while ((pos = str.find(key, pos)) != std::string::npos) {
        str.replace(pos, key.length(), value);
        pos += value.length();
    }
    return str;
}

/**
 * @brief Executes document generation from database and templates.
 *
 * Workflow:
 * - Parses CLI arguments (--source, --template, --output)
 * - Loads template file
 * - Fetches data from database
 * - Performs placeholder substitution
 * - Writes output files (TXT/MD or DOCX extraction/repackaging)
 *
 * @param ctx Parsed command-line context.
 * @return Exit code (0 on success, non-zero on failure).
 */
int GenerateCommand::execute(const CommandContext& ctx)
{
    auto sourceIt = ctx.args.find("--source");
    auto templateIt = ctx.args.find("--template");
    auto outputIt = ctx.args.find("--output");

    if (sourceIt == ctx.args.end() || templateIt == ctx.args.end()) {
        std::cerr << "Usage: generate --source <table> --template <name> [--output path]\n";
        return 1;
    }

    std::string table = sourceIt->second;
    std::string templateName = templateIt->second;

    std::string templateFile = findTemplateFile(templateName);
    if (templateFile.empty()) {
        std::cerr << "Template not found: " << templateName << "\n";
        return 1;
    }

    std::string tpl = loadFile(templateFile);
    if (tpl.empty()) {
        std::cerr << "Failed to load template\n";
        return 1;
    }

    Database db;

    auto columns = db.getColumns(table);
    auto rows = db.getRows(table, 10000);

    if (rows.empty()) {
        std::cerr << "No data in table: " << table << "\n";
        return 1;
    }

    std::string baseOutput;

    if (outputIt != ctx.args.end()) {
        baseOutput = outputIt->second;
    } else {
        const char* home = std::getenv("HOME");
        std::string homeDir = home ? home : ".";

        baseOutput = homeDir + "/Documents/markforge/output/" + getCurrentDateTime();
    }

    fs::create_directories(baseOutput);

    for (size_t r = 0; r < rows.size(); ++r) {
        std::string result = tpl;

        for (size_t i = 0; i < columns.size() && i < rows[r].size(); ++i) {
            std::string key = "{{" + columns[i] + "}}";
            result = replaceAll(result, key, rows[r][i]);
        }

        std::string id = (rows[r].size() > 0 ? rows[r][0] : std::to_string(r));
        std::string ext = fs::path(templateFile).extension().string();

        // ---------------- TXT / MD / OTHER TEXT FORMATS ----------------
        if (ext != ".docx") {
            std::string outFile = baseOutput + "/" + id + ext;

            std::ofstream out(outFile, std::ios::binary);
            out << result;
            out.close();
        } 
        // ---------------- DOCX HANDLING (OPTION 1: unzip/edit/zip) ----------------
        else {
            std::string tempDir = baseOutput + "/_tmp_" + id;
            std::string extracted = tempDir + "/doc";

            fs::create_directories(extracted);

            // copy template to temp and unzip
            std::string copyCmd = "cp \"" + templateFile + "\" \"" + tempDir + "/template.docx\"";
            std::string unzipCmd = "unzip -q \"" + tempDir + "/template.docx\" -d \"" + extracted + "\"";

            system(copyCmd.c_str());
            system(unzipCmd.c_str());

            std::string xmlPath = extracted + "/word/document.xml";

            std::ifstream in(xmlPath);
            std::stringstream buffer;
            buffer << in.rdbuf();
            std::string xml = buffer.str();
            in.close();

            // apply placeholder replacement (REUSING YOUR replaceAll LOGIC)
            for (size_t i = 0; i < columns.size() && i < rows[r].size(); ++i) {
                std::string key = "{{" + columns[i] + "}}";
                xml = replaceAll(xml, key, rows[r][i]);
            }

            std::ofstream out(xmlPath);
            out << xml;
            out.close();

            std::string outFile = baseOutput + "/" + id + ".docx";

            std::string zipCmd = "cd \"" + extracted + "\" && zip -qr \"" + outFile + "\" .";
            system(zipCmd.c_str());

            // optional cleanup
            std::string cleanupCmd = "rm -rf \"" + tempDir + "\"";
            system(cleanupCmd.c_str());
        }
    }

    std::cout << "Generated " << rows.size() << " files in: " << baseOutput << "\n";

    return 0;
}