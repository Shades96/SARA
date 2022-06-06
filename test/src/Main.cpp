#include <iostream>
#include <string>
#include <filesystem>

#include "Scanner.h"
#include "Output.h"

using std::string;

const string TEST_DIR_PATH = "tests";
const string SEP = "------------------------------\n";

int main()
{
    for (const auto &file : std::filesystem::directory_iterator(TEST_DIR_PATH)) {
        if (file.is_regular_file()) {
            Output::log() << SEP << file.path().string() << "\n" << SEP;

            Scanner scanner;
            std::ifstream stream{ file.path() };
            scanner.lex(stream);

            Output::log() << SEP;
        }
    }
}
