#include "Parser.h"

#include "Output.h"

#include <iostream>

int BracketPair::parse(const Terminal& t) 
{
	if (!stack.empty()) {
		auto err = stack.back().parse(t);
		if (stack.back().isComplete()) stack.pop_back();
		return err;
	}

	if (t.kind == T_OPEN) {
		if (empty) {
			empty = false;
			return EXIT_SUCCESS;
		}
		else {
			BracketPair inner{ T_OPEN, T_CLOSE };
			stack.push_back(inner);
			return stack.back().parse(t);
		}
	}
	else if (t.kind == T_CLOSE) {
		if (empty) {
			return EXIT_FAILURE;
		}
		else {
			Output::debug() << "Pair completed\n";
			complete = true;
			return EXIT_SUCCESS;
		}
	}
	return EXIT_FAILURE;
}

int Expression::parse(const Terminal& t)
{
	return EXIT_SUCCESS;
}

int Statement::parse(const Terminal& t)
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

int Block::parse(const Terminal& t)
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

const string ParameterList::SEPARATOR = ",";
int ParameterList::parse(const Terminal& t)
{
	// TODO
	auto err = delim.parse(t);
	if (delim.isComplete()) {
		complete = true;
	}
	return EXIT_SUCCESS;
}

int Function::parse(const Terminal& t)
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

int Program::parse(const Terminal& t)
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

int Parser::parse(Terminal& token)
{
	Output::debug() << Terminal::KIND_NAMES[token.kind] << "\n";
	return program.parse(token);
}