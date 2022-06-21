#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>

#include "Scanner.h"
#include "Parser.h"
#include "Compiler.h"
#include "Runtime.h"
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
            std::ifstream sourceStream{ file.path() };
            std::stringstream strbuf;
            strbuf << sourceStream.rdbuf();
            sourceStream.clear();
            sourceStream.seekg(0, std::ios::beg);

            string bytecodePath = OUTPUT_DIR_PATH + PATH_SEP + file.path().filename().string() + ".sara";
            Output::Bytecode::setOutfile(bytecodePath);

            Output::log() << SEP 
                << file.path().string() << " > " << bytecodePath << "\n" << SEP
                << strbuf.str() << "\n";

            Scanner scanner;
            Parser parser;
            auto parse = [&parser](Terminal t) { return parser.parse(t); };

            Output::log() << SEP << "Parsing source...\n";

            auto err = scanner.lex(sourceStream, parse);
            if (err) continue;

            Output::log() << SEP << "Compiling bytecode...\n";

            Compiler compiler;
            err = compiler.compile(parser.program);
            if (err) continue;

            Output::Bytecode::flush();
            Output::Bytecode::closeOutfile();

            Output::log() << SEP << "Running program...\n";

            std::ifstream bytecodeStream{ bytecodePath };
            vector<std::shared_ptr<Instruction>> instructions = Instruction::fromBytecode(bytecodeStream);
            Runtime runtime{ instructions };
            err = runtime.run();
            Output::log() << "\n";

            Output::log() << SEP << "Program terminated with status " << std::to_string(err) << "\n";

            Output::log() << SEP << "\n";
        }
    }
}
