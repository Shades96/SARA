#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>

#include "Parser.h"
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
            Parser parser;

            auto parse = [&parser](Terminal t) { return parser.parse(t); };

            scanner.lex(stream, parse);

            Output::log() << SEP;
        }
    }
}
