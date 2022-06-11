#pragma once

#include <string>
#include <vector>

using std::string, std::vector;

struct Terminal
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
	Terminal(Kind kind) :
		kind(kind)
	{}
	Kind kind;
	static const int MAX_IDENTIFIER_LEN;
	static const vector<string> KIND_MATCHERS;
	static const vector<string> KIND_NAMES;
};
