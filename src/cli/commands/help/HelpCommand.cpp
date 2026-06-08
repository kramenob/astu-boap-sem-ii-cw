/**
 * @file HelpCommand.cpp
 * @brief Implementation of CLI help system.
 *
 * Displays usage information, available commands,
 * and argument documentation.
 */
#include "HelpCommand.h"
/**
 * Project Module Imports
 *
 * Application metadata constants used in help output.
 */
#include "../../../core/About.h"
#include <iostream>
/**
 * @brief Displays CLI usage information and command reference.
 *
 * Prints application metadata and full command documentation.
 *
 * @param ctx Unused command context.
 * @return Exit code (always 0).
 */
int HelpCommand::execute(const CommandContext&)
{
	std::cout
	<< NAME << " " << VERSION << std::endl
	<< DESCRIPTION << std::endl
	<< R"(
USAGE:
  markforge <command> [options]

COMMANDS and OPTIONS:
  --help                                     Show this help message
  --version                                  Show application version

  db                                         Database operations
    --init                                   Create table in interactive mode
    --init --templates                       Show available table templates
    --init --template <teachers|students>    Initialize example database
    --drop <table_name>                      Delete table
    --drop --all                             Delete all tables
    --list                                   Show user created tables
    --list --all                             Show all tables
    --schema-show <table_name>               Show table schema

  data                                       Record operations
    --show                                   Show records in interactive mode
    --show <table_name>                      Show records from table

  template                                   Template operations
    --list                                   List templates
    --show <template_name>                   Show template
    --add <file> -as <template_name>         Register template
    --remove <template_name>                 Remove template

  generate                                   Generate documents
    --source <table_name>                    Source table
    --template <template_name>               Template to use
    --output <directory>                     Output directory

EXAMPLES:
  markforge db --init --template students
  markforge data --show students
  markforge template --list
  markforge generate --source students --template student_review --output ~/Documents/StudentReviews
)";

	return 0;

}