#include "HelpCommand.h"
#include <iostream>

int HelpCommand::execute(const CommandContext&)
{
    std::cout <<
R"(markforge - Document autofill tool

USAGE:
  markforge <command> [options]

COMMANDS:
  generate   Generate document from template and DB
  db         Database operations (init, reset)
  template   Template operations (WIP)
  help       Show this help message

EXAMPLES:
  markforge generate --id 1 --template invitation
  markforge db init
  markforge help
)";

    return 0;
}