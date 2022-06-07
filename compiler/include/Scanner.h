#pragma once

#include "Terminal.h"

#include <string>
#include <vector>
#include <fstream>
#include <functional>

using std::string, std::vector;

class Scanner
{
public:

	int lex(std::istream& input, std::function<int(Terminal)> outputToken);

private:
};
