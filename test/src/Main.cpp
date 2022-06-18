#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>

#include "Parser.h"
#include "Output.h"

using std::string;

const string PATH_SEP = "/";
const string TEST_DIR_PATH = "tests";
const string OUTPUT_DIR_PATH = TEST_DIR_PATH + PATH_SEP + "out";
const string SEP = "------------------------------\n";

int main()
{
    if (!std::filesystem::exists(TEST_DIR_PATH) || !std::filesystem::is_directory(TEST_DIR_PATH)) {
        Output::error() << "Test directory '" << std::filesystem::absolute(TEST_DIR_PATH).string() << "' does not exist\n";
        return EXIT_FAILURE;
    }
    if (!std::filesystem::exists(OUTPUT_DIR_PATH)) {
        std::filesystem::create_directories(OUTPUT_DIR_PATH);
    }
    else if (!std::filesystem::is_directory(OUTPUT_DIR_PATH)) {
        Output::error() << "Output directory '" << std::filesystem::absolute(OUTPUT_DIR_PATH).string() << "' cannot be created\n";
        return EXIT_FAILURE;
    }

    for (const auto &file : std::filesystem::directory_iterator(TEST_DIR_PATH)) {
        if (file.is_regular_file()) {
            std::ifstream stream{ file.path() };
            std::stringstream strbuf;
            strbuf << stream.rdbuf();
            stream.clear();
            stream.seekg(0, std::ios::beg);

            string outfilePath = OUTPUT_DIR_PATH + PATH_SEP + file.path().filename().string() + ".sara";
            Output::Bytecode::setOutfile(outfilePath);

            Output::log() << SEP 
                << file.path().string() << " > " << outfilePath << "\n" << SEP
                << strbuf.str() << "\n" << SEP;

            Scanner scanner;
            Parser parser;

            auto parse = [&parser](Terminal t) { return parser.parse(t); };

            scanner.lex(stream, parse);

            Output::log() << SEP;
        }
    }
}
