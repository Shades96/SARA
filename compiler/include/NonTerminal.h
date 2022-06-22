#pragma once

#include "Terminal.h"
#include "Output.h"
#include "Instruction.h"

#include <map>
#include <memory>

using std::unique_ptr;

struct Variable
{
	size_t stackLocation;
	size_t size = 1;
	enum Kind {
		PARAMETER,
		LOCAL
	} kind;
	Variable(Kind kind, size_t stackLocation) : kind(kind), stackLocation(stackLocation) {}
	Variable() : kind(Kind::LOCAL), stackLocation(0) {}
};

template <typename Data>
using symbol_table = std::map<string, Data>;
using function_table = symbol_table<size_t>;

struct BlockData
{
	symbol_table<Variable> variables;
	function_table functionRefs;
	size_t instrIndex = 0;
};
using BlockContext = std::shared_ptr<BlockData>;

class NonTerminal
{
public: 
	virtual int parse(const Terminal& t, BlockContext context) = 0;
	virtual int compile(BlockContext context) = 0;
	bool isEmpty() { return empty; }
	bool isComplete() { return complete; }
protected:
	bool empty = true;
	bool complete = false;
};

class BracketPair : public NonTerminal
{
public:
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
	BracketPair(Terminal::Kind T_OPEN, Terminal::Kind T_CLOSE)
		: T_OPEN(T_OPEN), T_CLOSE(T_CLOSE) {}
private:
	const Terminal::Kind T_OPEN, T_CLOSE;
};

class ParameterList : public NonTerminal
{
public:
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
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
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
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
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;

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
	virtual int parse(const Terminal& t, BlockContext context) override = 0;
	virtual int compile(BlockContext context) override = 0;
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
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
	FunctionCall() : Statement(Kind::FUNCTION_CALL) {};
private:
	int nestingDepth = 0;
	bool expectedParams = false;
	string funName = "";
	vector<Expression> params;
};

class Term : public NonTerminal
{
public:
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
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
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
	// make a copy of the outer scope and store it
	Block(BlockContext outer) : Statement(Kind::BLOCK), 
		context(std::make_shared<BlockContext::element_type>(*outer)) {};
	BlockContext context;
private:
	BracketPair delim { Terminal::Kind::CURLY_OPEN, Terminal::Kind::CURLY_CLOSE };
	vector<unique_ptr<Statement>> stmts;
	vector<Terminal> lookaheadBuf;
	int nestingDepth = 0;
};

class Return : public Statement
{
public:
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
	Return() : Statement(Kind::RETURN) {};
private:
	Expression expr;
};

class Branch : public Statement
{
public:
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
	Branch(BlockContext context) : Statement(Kind::BRANCH), body(context) {};
private:
	Expression cond;
	Block body;
};

class Loop : public Statement
{
public:
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
	Loop(BlockContext context) : Statement(Kind::LOOP), body(context) {};
private:
	Expression cond;
	Block body;
};

class Definition : public Statement
{
public:
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
	Definition() : Statement(Kind::DEFINITION) {};
private:
	bool expectedExpr = false;
	Terminal::Kind expectedTerm = Terminal::Kind::LET;
	string varName;
	Expression expr;
};

class Assignment : public Statement
{
public:
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
	Assignment() : Statement(Kind::ASSIGNMENT) {};
private:
	bool expectedExpr = false;
	LExpression lexpr;
	Expression expr;
};

class Function : public NonTerminal
{
public:
	int parse(const Terminal& t, BlockContext context) override;
	int compile(BlockContext context) override;
	Function(std::shared_ptr<function_table> functionRefs) : context(std::make_shared<BlockData>()) 
	{
		context->functionRefs = *functionRefs;
	}
	BlockContext context;
	string name;
	size_t entryPoint;
private:
	ParameterList params;
	std::unique_ptr<Block> body;
};

class Program
{
public:
	int parse(const Terminal& t);
	int compile();
	Program() : functionRefs(std::make_shared<function_table>()) 
	{
		(*functionRefs)["main"] = 0;
	}
	vector<Function> functions;
	std::shared_ptr<function_table> functionRefs;
	size_t instrIndex = 0;
};
