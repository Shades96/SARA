#pragma once

#include "Terminal.h"
#include "Output.h"

#include <memory>

using std::unique_ptr;

class NonTerminal
{
public: 
	virtual int parse(const Terminal& t) = 0;
	bool isComplete() { return complete; }
protected:
	bool complete = false;
};

class BracketPair : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
	bool isEmpty() { return empty; }
	BracketPair(Terminal::Kind T_OPEN, Terminal::Kind T_CLOSE)
		: T_OPEN(T_OPEN), T_CLOSE(T_CLOSE) {}
private:
	const Terminal::Kind T_OPEN, T_CLOSE;
	bool empty = true;
	vector<BracketPair> stack;
};

class Expression : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
};

class Statement : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
};

class Block : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
private:
	bool empty = true;
	BracketPair delim { Terminal::Kind::CURLY_OPEN, Terminal::Kind::CURLY_CLOSE };
	vector<Statement> stmts;
};

class ParameterList : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
private:
	static const Terminal::Kind SEPARATOR;
	BracketPair delim { Terminal::Kind::PARENTHESIS_OPEN, Terminal::Kind::PARENTHESIS_CLOSE };
	vector<string> params;
	bool idExpected = false;
	bool empty = true;
};

class Function : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
private:
	bool empty = true;
	string name;
	ParameterList params;
	Block body;
};

class Program : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
private:
	vector<Function> functions;
};