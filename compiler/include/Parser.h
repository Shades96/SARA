#pragma once

#include "Scanner.h"

class Parser
{
public:
	int parse(Scanner::Token &token);

private:
	std::vector<Scanner::Token> tokens;
};
