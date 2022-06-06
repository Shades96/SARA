#include "Scanner.h"

#include "Output.h"

#include <regex>

const int Scanner::Token::MAX_IDENTIFIER_LEN = 1024;
const vector<string> Scanner::Token::MATCH
{
    "//(.|[ \t])*",
    "!",
    "-",
    "\\+",
    "\\*",
    "/",
    "&&",
    "\\|\\|",
    "<",
    ">",
    "==",
    "=",
    ";",
    ",",
    "\\(",
    "\\)",
    "\\[",
    "\\]",
    "\\{",
    "\\}",
    "return",
    "if",
    "while",
    "let"
};

int Scanner::lex(std::istream& input)
{
    if (input.fail()) {
        Output::error() << "Bad input\n";
        return EXIT_FAILURE;
    }

    // read input line by line
    string remLine, wholeLine;
    int lineNum = 1;
    std::regex eatWhitespace("[ \t\n]+");
    while (std::getline(input, wholeLine)) {
        Output::debug() << wholeLine << "\n";
        remLine = wholeLine;

        while (remLine.length()) {
			// eat up leading whitespace
			std::smatch whitespace;
			std::regex_search(remLine, whitespace, eatWhitespace);
			if (whitespace.size() && !whitespace.prefix().length()) {
				remLine = whitespace.suffix().str();
			}

			// try and find a matching token
            bool matched = false;
			for (auto& matchStr : Scanner::Token::MATCH) {
				Output::debug() << "Scanning '" << remLine << "' for '" << matchStr << "'...\n";
				try {
					std::regex tokenRegex(matchStr);
					std::smatch match;
					std::regex_search(remLine, match, tokenRegex);
					if (match.size() && !match.prefix().length()) {
						Output::debug() << "Found " << matchStr << " ('" << match[0].str() << "')" << "\n";
                        remLine = match.suffix().str();
                        matched = true;
						break;
					}
				}
				catch (std::regex_error ex) {
					Output::error(lineNum) << "Regex error: " << ex.what() << "\n";
				}
			}

            if (!matched) {
                Output::error(lineNum) << "Unexpected symbol '" << remLine << "'\n";
                return EXIT_FAILURE;
            }
        }

        lineNum++;
    }
    return EXIT_SUCCESS;
}