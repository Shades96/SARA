#include <iostream>
#include <string>

#include "Scanner.h"

using std::cout;
using std::string;

const string PROG_NAME = "<program-name>";

void usage()
{
    cout << PROG_NAME << " { " << "<input-file>" << " }\n";
}

int main(int argc, char* argv[])
{
    Scanner scanner;
    switch (argc) {
    case 1: {
        scanner.lex(std::cin);
        break;
    }
    case 2: {
        std::ifstream input{ argv[1] };
        scanner.lex(input);
        break;
    }
    default: {
        usage();
        return EXIT_FAILURE;
        break;
    }
    }
}
