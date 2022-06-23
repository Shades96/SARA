#include "Scanner.h"

#include "Output.h"

#include <regex>

int Scanner::lex(std::istream& input, std::function<int(Terminal)> outputToken)
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
            int parseError = 0;
            for (int t = 0; t < Terminal::KIND_MATCHERS.size(); t++) {
                const string& matchStr = Terminal::KIND_MATCHERS[t];
				try {
					std::regex tokenRegex(matchStr);
					std::smatch match;
					std::regex_search(remLine, match, tokenRegex);
					if (match.size() && !match.prefix().length()) {
                        Terminal found((Terminal::Kind)t);
                        if (t != Terminal::Kind::SLASH_SLASH) {
                            if (t == Terminal::Kind::IDENTIFIER) {
                                found.id = match[0].str();
                            }
                            else if (t == Terminal::Kind::NUMBER) {
                                found.value = std::stol(match[0].str());
                            }
                            parseError = outputToken(found);
                        }

                        matched = true;
                        if (!parseError) {
                            remLine = match.suffix().str();
                        }
						break;
					}
				}
				catch (std::regex_error ex) {
					Output::error(lineNum) << ex.what() << matchStr << "\n";
				}
			}

            if (!matched) {
                Output::error(lineNum) << "Unexpected symbol '" << remLine << "'\n";
                return EXIT_FAILURE;
            }
            if (parseError) {
                Output::error(lineNum) << "Parser error: '" << remLine << "'\n";
                return EXIT_FAILURE;
            }
        }

        lineNum++;
    }
    return EXIT_SUCCESS;
}

const int Terminal::MAX_IDENTIFIER_LEN = 1024;
const vector<string> Terminal::KIND_MATCHERS
{
    "//(.|[ \t])*",
    "!=",
    "!",
    "-",
    "\\+",
    "\\*",
    "/",
    "%",
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
    "let",
    "[0-9]+",
    "[_a-zA-Z][_a-zA-Z0-9]*"
};
const vector<string> Terminal::KIND_NAMES
{
    "//",
    "!",
    "-",
    "+",
    "*",
    "/",
    "%",
    "&&",
    "||",
    "<",
    ">",
    "==",
    "=",
    ";",
    ",",
    "(",
    ")",
    "[",
    "]",
    "{",
    "}",
    "return",
    "if",
    "while",
    "let",
    "number",
    "identifier"
};