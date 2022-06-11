#pragma once

#include "NonTerminal.h"
#include "Scanner.h"

class Parser
{
public:
	int parse(Terminal &token);

private:
	Program program;
	std::vector<Terminal> tokens;
};
