#include <iostream>
#include <string>

#include "Parser.h"
#include "Output.h"

using std::string;

const string PROG_NAME = "<program-name>";

void usage()
{
    Output::error() << PROG_NAME << " { " << "<input-file>" << " }\n";
}

int main(int argc, char* argv[])
{
    Scanner scanner;
    Parser parser;

    auto parse = [&parser](Terminal t) { return parser.parse(t); };

    switch (argc) {
    case 1: {
        scanner.lex(std::cin, parse);
        break;
    }
    case 2: {
        std::ifstream input{ argv[1] };
        scanner.lex(input, parse);
        break;
    }
    default: {
        usage();
        return EXIT_FAILURE;
        break;
    }
    }
}
