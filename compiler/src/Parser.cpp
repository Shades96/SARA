#include "Parser.h"

#include "Output.h"

#include <iostream>

using std::cout;

int Parser::parse(Terminal& token)
{
    Output::debug() << Terminal::KIND_NAMES[token.kind] << "\n";
    return program.parse(token);
}