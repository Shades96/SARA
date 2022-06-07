#pragma once

#include "Scanner.h"

class Parser
{
public:
	int parse(Terminal &token);

private:
	std::vector<Terminal> tokens;
};
