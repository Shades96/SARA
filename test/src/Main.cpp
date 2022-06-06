#include <iostream>
#include <string>
#include <sstream>
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
            std::ifstream stream{ file.path() };
            std::stringstream strbuf;
            strbuf << stream.rdbuf();
            stream.clear();
            stream.seekg(0, std::ios::beg);

            Output::log() << SEP 
                << file.path().string() << "\n" << SEP
                << strbuf.str() << "\n" << SEP;

            Scanner scanner;
            scanner.lex(stream);

            Output::log() << SEP;
        }
    }
}
