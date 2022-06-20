#pragma once

#include "NonTerminal.h"

class Parser
{
public:
	int parse(const Terminal& token);
	Program program;
};
