#include "Parser.h"

#include "Output.h"

#include <iostream>

int BracketPair::parse(const Terminal& t) 
{
	if (t.kind == T_OPEN) {
		if (empty) {
			empty = false;
			return EXIT_SUCCESS;
		}
		else {
			return EXIT_FAILURE;
		}
	}
	else if (t.kind == T_CLOSE) {
		if (empty) {
			return EXIT_FAILURE;
		}
		else {
			complete = true;
			return EXIT_SUCCESS;
		}
	}
	return EXIT_FAILURE;
}

int Term::parse(const Terminal& t)
{
	if (empty) {
		switch (t.kind) {
		case Terminal::Kind::NUMBER:
			complete = true;
			return EXIT_SUCCESS;
			break;
		case Terminal::Kind::IDENTIFIER:
			kind = REFERENCE;
			id = t;
			return EXIT_SUCCESS;
			break;
		case Terminal::Kind::PARENTHESIS_OPEN:
			kind = EXPRESSION;
			exprDelim = std::make_unique<BracketPair>(
				Terminal::Kind::PARENTHESIS_OPEN, Terminal::Kind::PARENTHESIS_CLOSE);
			return exprDelim->parse(t);
			break;
		default:
			return EXIT_FAILURE;
			break;
		}
	}

	if (kind == REFERENCE) {
		switch (t.kind) {
		case Terminal::Kind::BRACKET_OPEN:
			kind = ARRAY_READ;
			exprDelim = std::make_unique<BracketPair>(
				Terminal::Kind::BRACKET_OPEN, Terminal::Kind::BRACKET_CLOSE);
			return exprDelim->parse(t);
			break;
		case Terminal::Kind::PARENTHESIS_OPEN: {
			kind = FUNCTION_CALL;
			funCall = std::make_unique<Call>();
			auto err = funCall->parse(id);
			if (err) return EXIT_FAILURE;
			return funCall->parse(t);
			break; }
		default:
			complete = true;
			return EXIT_FAILURE;
			break;
		}
	}

	if (kind == ARRAY_READ || kind == EXPRESSION) {
		auto err = exprDelim->parse(t);
		if (exprDelim->isComplete()) {
			if (!expr.isComplete()) {
				return EXIT_FAILURE;
			}
			else {
				complete = true;
				return err;
			}
		}
		return expr.parse(t);
	}

	if (kind == FUNCTION_CALL) {
		auto err = funCall->parse(t);
		if (funCall->isComplete()) {
			complete = true;
		}
		return err;
	}

	return EXIT_FAILURE;
}

// TODO
int Expression::parse(const Terminal& t)
{
	complete = true;
	return EXIT_SUCCESS;
}

// TODO
int LExpression::parse(const Terminal& t)
{
	complete = true;
	return EXIT_SUCCESS;
}

int Return::parse(const Terminal& t)
{
	switch (t.kind) {
	case Terminal::Kind::RETURN:
		if (empty) {
			empty = false;
			return EXIT_SUCCESS;
		} 
		else {
			return EXIT_FAILURE;
		}
		break;
	case Terminal::Kind::SEMICOLON:
		if (expr.isComplete()) {
			complete = true;
			return EXIT_SUCCESS;
		}
		else {
			return EXIT_FAILURE;
		}
		break;
	default:
		return expr.parse(t);
		break;
	}
}

int Branch::parse(const Terminal& t)
{
	if (empty) {
		if (t.kind != Terminal::Kind::IF) {
			return EXIT_FAILURE;
		}
		else {
			empty = false;
			return EXIT_SUCCESS;
		}
	}

	if (!cond.isComplete()) {
		return cond.parse(t);
	}

	auto err = block.parse(t);
	if (block.isComplete()) {
		complete = true;
	}
	return err;
}

int Loop::parse(const Terminal& t)
{
	if (empty) {
		if (t.kind != Terminal::Kind::WHILE) {
			return EXIT_FAILURE;
		}
		else {
			empty = false;
			return EXIT_SUCCESS;
		}
	}

	if (!cond.isComplete()) {
		return cond.parse(t);
	}

	auto err = block.parse(t);
	if (block.isComplete()) {
		complete = true;
	}
	return err;
}

int Definition::parse(const Terminal& t)
{
	if (!expectedExpr) {
		if (t.kind != expectedTerm) {
			return EXIT_FAILURE;
		}
		switch (t.kind)
		{
		case Terminal::Kind::LET:
			expectedTerm = Terminal::Kind::IDENTIFIER;
			break;
		case Terminal::Kind::IDENTIFIER:
			expectedTerm = Terminal::Kind::EQUALS;
			break;
		case Terminal::Kind::EQUALS:
			expectedExpr = true;
			break;
		default:
			return EXIT_FAILURE;
			break;
		}
		return EXIT_SUCCESS;
	}

	if (t.kind == Terminal::Kind::SEMICOLON) {
		if (expr.isComplete()) {
			complete = true;
			return EXIT_SUCCESS;
		}
		else {
			return EXIT_FAILURE;
		}
	}

	return expr.parse(t);
}

// TODO
int Assignment::parse(const Terminal& t)
{
	if (!lexpr.isComplete()) {
		return lexpr.parse(t);
	}

	if (!expectedExpr) {
		if (t.kind != Terminal::Kind::EQUALS) {
			return EXIT_FAILURE;
		}
		expectedExpr = true;
		return EXIT_SUCCESS;
	}

	if (t.kind == Terminal::Kind::SEMICOLON) {
		if (expr.isComplete()) {
			complete = true;
			return EXIT_SUCCESS;
		}
		else {
			return EXIT_FAILURE;
		}
	}

	return expr.parse(t);
}

// TODO
int Call::parse(const Terminal& t)
{
	complete = true;
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
		if (stmts.empty() || stmts.back()->isComplete()) {
			return EXIT_SUCCESS;
		}
		return EXIT_FAILURE;
	}

	if (stmts.empty() || stmts.back()->isComplete()) {
		switch (t.kind)
		{
		case Terminal::Kind::RETURN:
			stmts.push_back(std::make_unique<Return>());
			break;
		case Terminal::Kind::IF:
			stmts.push_back(std::make_unique<Branch>());
			break;
		case Terminal::Kind::WHILE:
			stmts.push_back(std::make_unique<Loop>());
			break;
		case Terminal::Kind::LET:
			stmts.push_back(std::make_unique<Definition>());
			break;
		case Terminal::Kind::CURLY_OPEN:
			stmts.push_back(std::make_unique<Block>());
			break;
		case Terminal::Kind::IDENTIFIER:
			// TODO: could be Assignment or function call
			stmts.push_back(std::make_unique<Assignment>());
			break;
		default:
			return EXIT_FAILURE;
			break;
		}
	}
	return stmts.back()->parse(t);
}

const Terminal::Kind ParameterList::SEPARATOR = Terminal::Kind::COMMA;
int ParameterList::parse(const Terminal& t)
{
	auto err = delim.parse(t);
	if (empty) {
		empty = false;
		return EXIT_SUCCESS;
	}
	if (delim.isComplete()) {
		if (idExpected) {
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected identifier\n";
			return EXIT_FAILURE;
		}
		complete = true;
		return EXIT_SUCCESS;
	}

	switch (t.kind) {
	case SEPARATOR:
		if (idExpected || params.empty()) {
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected identifier\n";
			return EXIT_FAILURE;
		}
		idExpected = true;
		return EXIT_SUCCESS;
		break;
	case Terminal::Kind::IDENTIFIER:
		if (!idExpected && !params.empty()) {
			Output::error() << "Unexpected " << Terminal::KIND_NAMES[t.kind] << "\n";
			return EXIT_FAILURE;
		}
		idExpected = false;
		params.push_back("param"); // TODO: save actual name
		return EXIT_SUCCESS;
		break;
	default:
		Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "'\n";
		return EXIT_FAILURE;
		break;
	}
}

int Function::parse(const Terminal& t)
{
	if (empty) {
		if (t.kind != Terminal::Kind::IDENTIFIER) {
			return EXIT_FAILURE;
		}
		empty = false;
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