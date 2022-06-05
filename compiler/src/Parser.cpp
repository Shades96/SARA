#include "Parser.h"

#include <iostream>

using std::cout;

int Parser::parse(std::istream &input)
{
    if (input.fail()) {
        cout << "Bad input\n";
        return EXIT_FAILURE;
    }
    while (input.good()) {
        char line[256];
        input.getline(line, 256);
        cout << line << "\n";
    }
    return EXIT_SUCCESS;
}