#pragma once

#include "NonTerminal.h"

class Parser
{
public:
	int parse(const Terminal& token);

private:
	Program program;
	std::vector<Terminal> tokens;
};
