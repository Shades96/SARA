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

class Term;
class Expression : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
private:
	enum Kind {
		TERM,
		NEGATION,
		NEGATIVE,
		ADDITION,
		SUBTRACTION,
		MULTIPLICATION,
		DIVISION,
		CONJUNCTION,
		DISJUNCTION,
		COMPARISON_LT,
		COMPARISON_EQ,
		COMPARISON_GT
	} kind = TERM;
	unique_ptr<Term> term1 = std::make_unique<Term>(), term2;
};

class LExpression : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
private:
	enum Kind {
		VAR_WRITE,
		ARRAY_WRITE,
	} kind;
	Terminal id;
	Expression expr;
	unique_ptr<BracketPair> exprDelim;
};

class Statement : public NonTerminal
{
public:
	virtual int parse(const Terminal& t) override = 0;
	enum Kind {
		RETURN = 0,
		BRANCH,
		LOOP,
		DEFINITION,
		ASSIGNMENT,
		FUNCTION_CALL,
		BLOCK
	} kind;
	Statement(Kind kind) : kind(kind) {}
};

class Call : public Statement
{
public:
	int parse(const Terminal& t) override;
	Call() : Statement(Kind::FUNCTION_CALL) {};
private:
	int nestingDepth = 0;
	bool expectedParams = false;
	vector<Expression> params;
};

class Term : public NonTerminal
{
public:
	int parse(const Terminal& t) override;
	enum Kind {
		NUMBER = 0,
		REFERENCE,
		ARRAY_READ,
		FUNCTION_CALL,
		EXPRESSION
	} kind;
private:
	Terminal id;
	unique_ptr<Expression> expr;
	unique_ptr<BracketPair> exprDelim;
	unique_ptr<Call> funCall;
};

class Block : public Statement
{
public:
	int parse(const Terminal& t) override;
	Block() : Statement(Kind::BLOCK) {};
private:
	BracketPair delim { Terminal::Kind::CURLY_OPEN, Terminal::Kind::CURLY_CLOSE };
	vector<unique_ptr<Statement>> stmts;
	vector<Terminal> lookaheadBuf;
	int nestingDepth = 0;
};

class Return : public Statement
{
public:
	int parse(const Terminal& t) override;
	Return() : Statement(Kind::RETURN) {};
private:
	Expression expr;
};

class Branch : public Statement
{
public:
	int parse(const Terminal& t) override;
	Branch() : Statement(Kind::BRANCH) {};
private:
	Expression cond;
	Block block;
};

class Loop : public Statement
{
public:
	int parse(const Terminal& t) override;
	Loop() : Statement(Kind::LOOP) {};
private:
	Expression cond;
	Block block;
};

class Definition : public Statement
{
public:
	int parse(const Terminal& t) override;
	Definition() : Statement(Kind::DEFINITION) {};
private:
	Terminal::Kind expectedTerm = Terminal::Kind::LET;
	bool expectedExpr = false;
	Expression expr;
};

class Assignment : public Statement
{
public:
	int parse(const Terminal& t) override;
	Assignment() : Statement(Kind::ASSIGNMENT) {};
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