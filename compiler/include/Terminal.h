#pragma once

#include "NonTerminal.h"

class Terminal : public NonTerminal
{
public:
	Terminal(int kind) :
		kind(kind),
		NonTerminal({})
	{}
private:
	int kind;
};
