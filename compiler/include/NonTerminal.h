#pragma once

#include "Scanner.h"

using std::vector;

class NonTerminal
{
public:
	NonTerminal(vector<NonTerminal> pattern) : 
		pattern(pattern)
	{}
private:
	vector<NonTerminal> pattern;
};

class Optional : public NonTerminal
{

};

class Repeat : public NonTerminal
{

};