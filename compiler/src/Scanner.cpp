#include "Scanner.h"

#include "Output.h"

#include <iostream>

using std::cout;

int Scanner::lex(std::istream& input)
{
    if (input.fail()) {
        Output::error() << "Bad input\n";
        return EXIT_FAILURE;
    }
    while (input.good()) {
        char line[256];
        input.getline(line, 256);
        Output::log() << line << "\n";
    }
    return EXIT_SUCCESS;
}