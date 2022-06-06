#pragma once

#include <string>
#include <vector>
#include <fstream>

using std::string, std::vector;

class Scanner
{
public:
	struct Token
	{
		enum Kind
		{
			// SYMBOLS
			SLASH_SLASH = 0,
			BANG,
			DASH,
			PLUS,
			STAR,
			SLASH,
			AND_AND,
			OR_OR,
			LESS_THAN,
			GREATER_THAN,
			EQUALS_EQUALS,
			EQUALS,
			SEMICOLON,
			COMMA,
			PARENTHESIS_OPEN,
			PARENTHESIS_CLOSE,
			BRACKET_OPEN,
			BRACKET_CLOSE,
			CURLY_OPEN,
			CURLY_CLOSE,
			// KEYWORDS
			RETURN,
			IF,
			WHILE,
			LET,
			// CONSTANTS
			NUMBER,
			// ID
			IDENTIFIER,
		};
		static const int MAX_IDENTIFIER_LEN;
		static const vector<string> MATCH;
	};

	int lex(std::istream& input);

private:
};
