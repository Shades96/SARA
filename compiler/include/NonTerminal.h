#pragma once

#include "Terminal.h"
#include "Output.h"
#include "Instruction.h"

#include <map>
#include <memory>

using std::unique_ptr;

struct FunctionData
{
	int entryPoint;
};

struct VariableData
{
	int stackLocation;
};

template <typename Data>
using symbol_table = std::map<string, Data>;

struct ProgramContext
{
	symbol_table<FunctionData> functions;
	symbol_table<VariableData> variables;
};

class NonTerminal
{
public: 
	virtual int parse(const Terminal& t, ProgramContext& context) = 0;
	bool isEmpty() { return empty; }
	bool isComplete() { return complete; }
protected:
	bool empty = true;
	bool complete = false;
};

class BracketPair : public NonTerminal
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
	BracketPair(Terminal::Kind T_OPEN, Terminal::Kind T_CLOSE)
		: T_OPEN(T_OPEN), T_CLOSE(T_CLOSE) {}
private:
	const Terminal::Kind T_OPEN, T_CLOSE;
};

class ParameterList : public NonTerminal
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
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
	int parse(const Terminal& t, ProgramContext& context) override;
private:
	enum Kind {
		TERM,
		NEGATION,
		NEGATIVE,
		ADDITION,
		SUBTRACTION,
		MULTIPLICATION,
		DIVISION,
		MODULO,
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
	int parse(const Terminal& t, ProgramContext& context) override;
private:
	enum Kind {
		VAR_WRITE,
		ARRAY_WRITE,
	} kind = VAR_WRITE;
	Terminal id;
	Expression expr;
	unique_ptr<BracketPair> exprDelim;
};

class Statement : public NonTerminal
{
public:
	virtual int parse(const Terminal& t, ProgramContext& context) override = 0;
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

class FunctionCall : public Statement
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
	FunctionCall() : Statement(Kind::FUNCTION_CALL) {};
private:
	int nestingDepth = 0;
	bool expectedParams = false;
	vector<Expression> params;
};

class Term : public NonTerminal
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
	enum Kind {
		NUMBER = 0,
		REFERENCE,
		ARRAY_READ,
		FUNCTION_CALL,
		EXPRESSION
	} kind = NUMBER;
private:
	Terminal id;
	unique_ptr<Expression> expr;
	unique_ptr<BracketPair> exprDelim;
	unique_ptr<FunctionCall> funCall;
};

class Block : public Statement
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
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
	int parse(const Terminal& t, ProgramContext& context) override;
	Return() : Statement(Kind::RETURN) {};
private:
	Expression expr;
};

class Branch : public Statement
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
	Branch() : Statement(Kind::BRANCH) {};
private:
	Expression cond;
	Block block;
};

class Loop : public Statement
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
	Loop() : Statement(Kind::LOOP) {};
private:
	Expression cond;
	Block block;
};

class Definition : public Statement
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
	Definition() : Statement(Kind::DEFINITION) {};
private:
	Terminal::Kind expectedTerm = Terminal::Kind::LET;
	bool expectedExpr = false;
	Expression expr;
};

class Assignment : public Statement
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
	Assignment() : Statement(Kind::ASSIGNMENT) {};
private:
	bool expectedExpr = false;
	LExpression lexpr;
	Expression expr;
};

class Function : public NonTerminal
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
private:
	string name;
	ParameterList params;
	Block body;
};

class Program : public NonTerminal
{
public:
	int parse(const Terminal& t, ProgramContext& context) override;
private:
	vector<Function> functions;
};