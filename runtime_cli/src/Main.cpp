#include "Runtime.h"
#include "Output.h"

const string PROG_NAME = "<program-name>";

void usage()
{
    Output::error() << PROG_NAME << " { " << "<input-file>" << " }\n";
}

int main(int argc, char* argv[])
{
    instr_seq instructions;
    switch (argc) {
    case 1: {
        instructions = Instruction::fromBytecode(std::cin);
        break;
    }
    case 2: {
        std::ifstream input{ argv[1] };
        if (input.fail()) {
            Output::error() << "Bad input\n";
            return EXIT_FAILURE;
        }
        instructions = Instruction::fromBytecode(input);
        break;
    }
    default: {
        usage();
        return EXIT_FAILURE;
        break;
    }
    }

    Runtime runtime{ instructions };
    return runtime.run();
}
