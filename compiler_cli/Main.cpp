#include <iostream>
#include <string>

#include "Parser.h"

using std::cout;
using std::string;

const string PROG_NAME = "<program-name>";

void usage()
{
    cout << PROG_NAME << " { " << "<input-file>" << " }\n";
}

int main(int argc, char* argv[])
{
    Parser parser;
    switch (argc) {
    case 1: {
        parser.parse(std::cin);
        break;
    }
    case 2: {
        std::ifstream input{ argv[1] };
        parser.parse(input);
        break;
    }
    default: {
        usage();
        return EXIT_FAILURE;
        break;
    }
    }
}
