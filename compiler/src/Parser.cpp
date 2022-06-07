#include "Parser.h"

#include "Output.h"

#include <iostream>

using std::cout;

int Parser::parse(Scanner::Token& token)
{
    Output::log() << Scanner::Token::KIND_NAMES[token.kind] << "\n";
    return EXIT_SUCCESS;
}