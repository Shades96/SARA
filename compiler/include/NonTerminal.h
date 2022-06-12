#pragma once

#include "Terminal.h"
#include "Output.h"

#include <memory>

using std::unique_ptr;

class NonTerminal
{
public: 
	virtual int parse(const Terminal& t) = 0;
	bool isEmpty() { return empty; }
	bool isComplete() { return complete; }
protected:
	bool empty = true;
	bool complete = false;
};

class BracketPair : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
	BracketPair(Terminal::Kind T_OPEN, Terminal::Kind T_CLOSE)
		: T_OPEN(T_OPEN), T_CLOSE(T_CLOSE) {}
private:
	const Terminal::Kind T_OPEN, T_CLOSE;
};

class ParameterList : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
private:
	static const Terminal::Kind SEPARATOR;
	BracketPair delim{ Terminal::Kind::PARENTHESIS_OPEN, Terminal::Kind::PARENTHESIS_CLOSE };
	vector<string> params;
	bool idExpected = false;
};

class Expression : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
};

class LExpression : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
};

class Statement : public NonTerminal
{
public:
	virtual int parse(const Terminal& t) override = 0;
};

class Call : public Statement
{
public:
	int parse(const Terminal& t) override;
};

class Term : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
private:
	enum Kind {
		NUMBER = 0,
		REFERENCE,
		ARRAY_READ,
		FUNCTION_CALL,
		EXPRESSION
	} kind;
	Terminal id;
	Expression expr;
	unique_ptr<BracketPair> exprDelim;
	unique_ptr<Call> funCall;
};

class Block : public Statement
{
public:
	int parse(const Terminal& t) override;
private:
	BracketPair delim { Terminal::Kind::CURLY_OPEN, Terminal::Kind::CURLY_CLOSE };
	vector<unique_ptr<Statement>> stmts;
};

class Return : public Statement
{
public:
	int parse(const Terminal& t) override;
private:
	Expression expr;
};

class Branch : public Statement
{
public:
	int parse(const Terminal& t) override;
private:
	Expression cond;
	Block block;
};

class Loop : public Statement
{
public:
	int parse(const Terminal& t) override;
private:
	Expression cond;
	Block block;
};

class Definition : public Statement
{
public:
	int parse(const Terminal& t) override;
private:
	Terminal::Kind expectedTerm = Terminal::Kind::LET;
	bool expectedExpr = false;
	Expression expr;
};

class Assignment : public Statement
{
public:
	int parse(const Terminal& t) override;
private:
	bool expectedExpr = false;
	LExpression lexpr;
	Expression expr;
};

class Function : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
private:
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