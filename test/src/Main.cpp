#include <iostream>
#include <string>
#include <filesystem>

#include "Parser.h"

using std::cout;
using std::string;

const string TEST_DIR_PATH = "tests";
const string SEP = "------------------------------\n";

int main()
{
    for (const auto &file : std::filesystem::directory_iterator(TEST_DIR_PATH)) {
        if (file.is_regular_file()) {
            cout << SEP << file.path() << "\n" << SEP;

            Parser parser;
            std::ifstream stream{ file.path() };
            parser.parse(stream);

            cout << SEP;
        }
    }
}
