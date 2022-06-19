#pragma once

#include "NonTerminal.h"

class Parser
{
public:
	int parse(Terminal& token);

private:
	Program program;
	ProgramContext context;
	std::vector<Terminal> tokens;
};
