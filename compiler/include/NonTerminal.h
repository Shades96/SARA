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

template<Terminal::Kind	T_OPEN, Terminal::Kind T_CLOSE>
class BracketPair : public NonTerminal
{
public:
	int parse(const Terminal& t)
	{
		if (!stack.empty()) {
			auto err = stack.back().parse(t);
			if (stack.back().isComplete()) stack.pop_back();
			return err;
		}

		switch (t.kind) {
		case T_OPEN:
			if (empty) {
				empty = false;
				return EXIT_SUCCESS;
			}
			else {
				BracketPair inner;
				stack.push_back(inner);
				return stack.back().parse(t);
			}
			break;
		case T_CLOSE:
			if (empty) {
				return EXIT_FAILURE;
			}
			else {
				Output::debug() << "Pair completed\n";
				complete = true;
				return EXIT_SUCCESS;
			}
			break;
		default: 
			return EXIT_FAILURE;
			break;
		}
	}
	bool isEmpty() { return empty; }
private:
	bool empty = true;
	vector<BracketPair> stack;
};

class Expression : public NonTerminal
{
public:
	int parse(const Terminal& t) override
	{
		return EXIT_SUCCESS;
	}
};

class Statement : public NonTerminal
{
public:
	int parse(const Terminal& t) override
	{
		switch (t.kind) {
		case Terminal::Kind::SEMICOLON:
			complete = true;
			break;
		default:
			break;
		}
		return EXIT_SUCCESS;
	}
private:
};

class Block : public NonTerminal
{
public:
	int parse(const Terminal& t) override
	{
		auto err = delim.parse(t);
		if (empty) {
			empty = false;
			return err;
		}
		if (delim.isComplete()) {
			complete = true;
			if (stmts.empty() || stmts.back().isComplete()) {
				return EXIT_SUCCESS;
			}
			return EXIT_FAILURE;
		}

		if (stmts.empty() || stmts.back().isComplete()) {
			stmts.push_back(Statement{});
		}
		return stmts.back().parse(t);
	}
private:
	bool empty = true;
	BracketPair<Terminal::Kind::CURLY_OPEN, Terminal::Kind::CURLY_CLOSE> delim;
	vector<Statement> stmts;
};

class ParameterList : public NonTerminal
{
public:
	int parse(const Terminal& t) override
	{
		// TODO
		auto err = delim.parse(t);
		if (delim.isComplete()) {
			complete = true;
		}
		return EXIT_SUCCESS;
	}
private:
	BracketPair<Terminal::Kind::PARENTHESIS_OPEN, Terminal::Kind::PARENTHESIS_CLOSE> delim;
	vector<string> params;
	bool empty = true;
};

class Function : public NonTerminal
{
public:
	int parse(const Terminal& t) override
	{
		if (empty) {
			empty = false;
			if (t.kind != Terminal::Kind::IDENTIFIER) {
				return EXIT_FAILURE;
			}
			return EXIT_SUCCESS;
		}

		if (!params.isComplete()) {
			return params.parse(t);
		}

		auto err = body.parse(t);
		if (body.isComplete()) {
			complete = true;
		}
		return err;
	}
private:
	bool empty = true;
	string name;
	ParameterList params;
	Block body;
};

class Program : public NonTerminal
{
public:
	int parse(const Terminal& t) override
	{
		if (functions.empty()) {
			functions.push_back(Function{});
		}
		auto err = functions.back().parse(t);
		if (functions.back().isComplete()) {
			functions.pop_back();
		}
		return err;
	}
private:
	vector<Function> functions;
};