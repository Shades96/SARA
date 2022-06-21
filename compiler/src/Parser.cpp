#include "Parser.h"

#include "Output.h"

#include <iostream>

int BracketPair::parse(const Terminal& t, BlockContext context) 
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

int Term::parse(const Terminal& t, BlockContext context)
{
	if (empty) {
		empty = false;
		switch (t.kind) {
		case Terminal::Kind::NUMBER:
			kind = NUMBER;
			id = t;
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
			expr = std::make_unique<Expression>();
			exprDelim = std::make_unique<BracketPair>(
				Terminal::Kind::PARENTHESIS_OPEN, Terminal::Kind::PARENTHESIS_CLOSE);
			return exprDelim->parse(t, context);
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
			expr = std::make_unique<Expression>();
			exprDelim = std::make_unique<BracketPair>(
				Terminal::Kind::BRACKET_OPEN, Terminal::Kind::BRACKET_CLOSE);
			return exprDelim->parse(t, context);
			break;
		case Terminal::Kind::PARENTHESIS_OPEN: {
			kind = FUNCTION_CALL;
			funCall = std::make_unique<FunctionCall>();
			auto err = funCall->parse(id, context);
			if (err) return EXIT_FAILURE;
			return funCall->parse(t, context);
			break; }
		default:
			complete = true;
			return EXIT_FAILURE;
			break;
		}
	}

	if (kind == ARRAY_READ || kind == EXPRESSION) {
		auto err = exprDelim->parse(t, context);
		if (exprDelim->isComplete()) {
			if (!expr->isComplete()) {
				Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected expression\n";
				return EXIT_FAILURE;
			}
			else {
				complete = true;
				return err;
			}
		}
		return expr->parse(t, context);
	}

	if (kind == FUNCTION_CALL) {
		auto err = funCall->parse(t, context);
		if (funCall->isComplete()) {
			complete = true;
		}
		return err;
	}

	return EXIT_FAILURE;
}

int Expression::parse(const Terminal& t, BlockContext context)
{
	if (empty) {
		empty = false;
		switch (t.kind)
		{
		case Terminal::Kind::BANG:
			kind = NEGATION;
			return EXIT_SUCCESS;
			break;
		case Terminal::Kind::DASH:
			kind = NEGATIVE;
			return EXIT_SUCCESS;
			break;
		default:
			auto err = term1->parse(t, context);
			if (term1->isComplete()) {
				complete = true;
			}
			return err;
			break;
		}
	}

	if (!term1->isComplete()) {
		auto err = term1->parse(t, context);
		if (!term1->isComplete()) {
			return err;
		}
		if (kind == NEGATION || kind == NEGATIVE) {
			complete = true;
			return err;
		}
		if (term1->kind == Term::Kind::EXPRESSION) {
			if (t.kind == Terminal::Kind::PARENTHESIS_CLOSE) {
				return err;
			}
			else {
				Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected " << Terminal::KIND_NAMES[Terminal::Kind::PARENTHESIS_CLOSE] << "\n";
				return EXIT_FAILURE;
			}
		}
		if (term1->kind == Term::Kind::ARRAY_READ) {
			if (t.kind == Terminal::Kind::BRACKET_CLOSE) {
				return err;
			}
			else {
				Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected " << Terminal::KIND_NAMES[Terminal::Kind::BRACKET_CLOSE] << "\n";
				return EXIT_FAILURE;
			}
		}
	}

	if (term2) {
		auto err = term2->parse(t, context);
		if (term2->isComplete()) {
			complete = true;
		}
		return err;
	}

	switch (t.kind)
	{
	case Terminal::Kind::PLUS:
		kind = ADDITION;
		break;
	case Terminal::Kind::DASH:
		kind = SUBTRACTION;
		break;
	case Terminal::Kind::STAR:
		kind = MULTIPLICATION;
		break;
	case Terminal::Kind::SLASH:
		kind = DIVISION;
		break;
	case Terminal::Kind::PERCENT:
		kind = MODULO;
		break;
	case Terminal::Kind::AND_AND:
		kind = CONJUNCTION;
		break;
	case Terminal::Kind::OR_OR:
		kind = DISJUNCTION;
		break;
	case Terminal::Kind::LESS_THAN:
		kind = COMPARISON_LT;
		break;
	case Terminal::Kind::EQUALS_EQUALS:
		kind = COMPARISON_EQ;
		break;
	case Terminal::Kind::GREATER_THAN:
		kind = COMPARISON_GT;
		break;
	default:
		complete = true;
		return EXIT_FAILURE;
		break;
	}
	term2 = std::make_unique<Term>();
	return EXIT_SUCCESS;
}

int LExpression::parse(const Terminal& t, BlockContext context)
{
	if (empty) {
		if (t.kind != Terminal::Kind::IDENTIFIER) {
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected identifier\n";
			return EXIT_FAILURE;
		}
		empty = false;
		id = t;
		return EXIT_SUCCESS;
	}

	if (kind == ARRAY_WRITE) {
		auto err = exprDelim->parse(t, context);
		if (exprDelim->isComplete()) {
			if (!expr.isComplete()) {
				Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected expression\n";
				return EXIT_FAILURE;
			}
			else {
				complete = true;
				return err;
			}
		}
		return expr.parse(t, context);
	}

	switch (t.kind)
	{
	case Terminal::Kind::BRACKET_OPEN:
		kind = ARRAY_WRITE;
		exprDelim = std::make_unique<BracketPair>(
			Terminal::Kind::BRACKET_OPEN, Terminal::Kind::BRACKET_CLOSE);
		return exprDelim->parse(t, context);
		break;
	default:
		kind = VAR_WRITE;
		complete = true;
		return EXIT_FAILURE;
		break;
	}

	return EXIT_FAILURE;
}

int FunctionCall::parse(const Terminal& t, BlockContext context)
{
	if (empty) {
		if (t.kind != Terminal::Kind::IDENTIFIER) {
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected identifier\n";
			return EXIT_FAILURE;
		}
		empty = false;
		expectedParams = true;
		funName = t.id;
		return EXIT_SUCCESS;
	}
	
	if (expectedParams) {
		if (t.kind != Terminal::Kind::PARENTHESIS_OPEN) {
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected " << Terminal::KIND_NAMES[Terminal::Kind::PARENTHESIS_OPEN] << "\n";
			return EXIT_FAILURE;
		}
		expectedParams = false;
		return EXIT_SUCCESS;
	}

	switch (t.kind) {
	case Terminal::Kind::PARENTHESIS_OPEN:
		if (params.empty() || params.back().isComplete()) {
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected expression\n";
			return EXIT_FAILURE;
		}
		nestingDepth++;
		return params.back().parse(t, context);
		break;
	case Terminal::Kind::PARENTHESIS_CLOSE:
		if (nestingDepth == 0) {
			if (!params.empty() && !params.back().isComplete()) {
				auto err = params.back().parse(t, context);
			}
			if (params.empty() || params.back().isComplete()) {
				if (context->functionRefs.find(funName) == context->functionRefs.end()) {
					// TODO: check that assigned size is correct
					context->functionRefs[funName] = context->functionRefs.size();
				}
				Output::debug() << "Call statement complete\n";
				complete = true;
				return EXIT_SUCCESS;
			}
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected expression\n";
			return EXIT_FAILURE;
		}
		nestingDepth--;
		break;
	case Terminal::Kind::COMMA:
		if (nestingDepth == 0) {
			if (!params.empty() && params.back().isComplete()) {
				Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected expression\n";
				return EXIT_FAILURE;
			}
			return EXIT_SUCCESS;
		}
		break;
	default:
		break;
	}

	if (params.empty() || params.back().isComplete()) {
		params.push_back(Expression());
	}
	auto err = params.back().parse(t, context);
	if (err && params.back().isComplete()) {
		return this->parse(t, context);
	}
	return err;
}

int Return::parse(const Terminal& t, BlockContext context)
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
			Output::debug() << "Return statement complete\n";
			return EXIT_SUCCESS;
		}
		else {
			auto err = expr.parse(t, context);
			if (expr.isComplete()) {
				complete = true;
				Output::debug() << "Return statement complete\n";
				return EXIT_SUCCESS;
			}
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected expression\n";
			return EXIT_FAILURE;
		}
		break;
	default:
		break;
	}

	auto err = expr.parse(t, context);
	if (expr.isComplete()) {
		return EXIT_SUCCESS;
	}
	return err;
}

int Branch::parse(const Terminal& t, BlockContext context)
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
		return cond.parse(t, context);
	}

	auto err = body.parse(t, context);
	if (body.isComplete()) {
		Output::debug() << "Branch statement complete\n";
		complete = true;
	}
	return err;
}

int Loop::parse(const Terminal& t, BlockContext context)
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
		// TODO catch
		auto err = cond.parse(t, context);
		if (cond.isComplete()) {
			if (err) {
				return this->parse(t, context);
			}
			return EXIT_SUCCESS;
		}
		return err;
	}

	auto err = body.parse(t, context);
	if (body.isComplete()) {
		Output::debug() << "Loop statement complete\n";
		complete = true;
	}
	return err;
}

int Definition::parse(const Terminal& t, BlockContext context)
{
	if (!expectedExpr) {
		if (t.kind != expectedTerm) {
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected " << Terminal::KIND_NAMES[expectedTerm] << "\n";
			return EXIT_FAILURE;
		}
		switch (t.kind)
		{
		case Terminal::Kind::LET:
			expectedTerm = Terminal::Kind::IDENTIFIER;
			break;
		case Terminal::Kind::IDENTIFIER:
			expectedTerm = Terminal::Kind::EQUALS;
			context->variables.insert(std::make_pair(t.id, Variable {
				Variable::Kind::LOCAL, context->variables.size()
			}));
			varName = t.id;
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
		if (!expr.isComplete()) {
			auto err = expr.parse(t, context);
		}
		if (expr.isComplete()) {
			Output::debug() << "Definition statement complete\n";
			complete = true;
			return EXIT_SUCCESS;
		}
		Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected expression\n";
		return EXIT_FAILURE;
	}

	auto err = expr.parse(t, context);
	if (expr.isComplete()) {
		return EXIT_SUCCESS;
	}
	return err;
}

int Assignment::parse(const Terminal& t, BlockContext context)
{
	if (!lexpr.isComplete()) {
		auto err = lexpr.parse(t, context);
		if (!lexpr.isComplete() || !err) {
			return err;
		}
	}

	if (!expectedExpr) {
		if (t.kind != Terminal::Kind::EQUALS) {
			return EXIT_FAILURE;
		}
		expectedExpr = true;
		return EXIT_SUCCESS;
	}

	if (t.kind == Terminal::Kind::SEMICOLON) {
		if (!expr.isComplete()) {
			auto err = expr.parse(t, context);
		}
		if (expr.isComplete()) {
			Output::debug() << "Assignment statement complete\n";
			complete = true;
			return EXIT_SUCCESS;
		}
		Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected expression\n";
		return EXIT_FAILURE;
	}

	auto err = expr.parse(t, context);
	if (expr.isComplete()) {
		return EXIT_SUCCESS;
	}
	return err;
}

int Block::parse(const Terminal& t, BlockContext context)
{
	if (nestingDepth == 0) {
		auto err = delim.parse(t, this->context);
		if (empty) {
			empty = false;
			return err;
		}
		if (delim.isComplete()) {
			Output::debug() << "Block statement complete\n";
			complete = true;
			if (stmts.empty() || stmts.back()->isComplete()) {
				return EXIT_SUCCESS;
			}
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected statement\n";
			return EXIT_FAILURE;
		}
	}
	if (t.kind == Terminal::Kind::CURLY_OPEN) {
		nestingDepth++;
	}
	else if (t.kind == Terminal::Kind::CURLY_CLOSE && nestingDepth > 0) {
		nestingDepth--;
	}

	if (stmts.empty() || stmts.back()->isComplete()) {
		lookaheadBuf.clear();
		switch (t.kind)
		{
		case Terminal::Kind::RETURN:
			stmts.push_back(std::make_unique<Return>());
			break;
		case Terminal::Kind::IF:
			stmts.push_back(std::make_unique<Branch>(this->context));
			break;
		case Terminal::Kind::WHILE:
			stmts.push_back(std::make_unique<Loop>(this->context));
			break;
		case Terminal::Kind::LET:
			stmts.push_back(std::make_unique<Definition>());
			break;
		case Terminal::Kind::CURLY_OPEN:
			stmts.push_back(std::make_unique<Block>(this->context));
			break;
		case Terminal::Kind::IDENTIFIER:
			// could be Assignment or function call
			lookaheadBuf.push_back(t);
			stmts.push_back(std::make_unique<Assignment>());
			break;
		case Terminal::Kind::SEMICOLON:
			if (stmts.empty() || stmts.back()->kind == Statement::Kind::FUNCTION_CALL) {
				return EXIT_SUCCESS;
			}
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected statement\n";
			return EXIT_FAILURE;
			break;
		default:
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected statement\n";
			return EXIT_FAILURE;
			break;
		}
	}

	auto err = stmts.back()->parse(t, this->context);
	if (err && lookaheadBuf.size()) {
		stmts.pop_back();
		stmts.push_back(std::make_unique<FunctionCall>());
		err = stmts.back()->parse(lookaheadBuf[0], this->context);
		if (err) return err;
		return stmts.back()->parse(t, this->context);
	}
	return err;
}

const Terminal::Kind ParameterList::SEPARATOR = Terminal::Kind::COMMA;
int ParameterList::parse(const Terminal& t, BlockContext context)
{
	auto err = delim.parse(t, context);
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
		context->variables.insert(std::make_pair(t.id, Variable {
			Variable::Kind::PARAMETER, params.size()
		}));
		params.push_back(t.id);
		return EXIT_SUCCESS;
		break;
	default:
		Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "'\n";
		return EXIT_FAILURE;
		break;
	}
}

int Function::parse(const Terminal& t, BlockContext context)
{
	if (empty) {
		if (t.kind != Terminal::Kind::IDENTIFIER) {
			Output::error() << "Unexpected '" << Terminal::KIND_NAMES[t.kind] << "' - expected identifier\n";
			return EXIT_FAILURE;
		}
		empty = false;
		name = t.id;
		return EXIT_SUCCESS;
	}

	if (!params.isComplete()) {
		return params.parse(t, context);
	}

	if (!body) {
		body = std::make_unique<Block>(context);
	}

	auto err = body->parse(t, context);
	if (body->isComplete()) {
		Output::debug() << "Function definition complete\n";
		complete = true;
	}
	return err;
}

int Program::parse(const Terminal& t)
{
	if (functions.empty() || functions.back().isComplete()) {
		functions.push_back(Function{ functionRefs });
	}
	auto err = functions.back().parse(t, functions.back().context);
	return err;
}

int Parser::parse(const Terminal& token)
{
	return program.parse(token);
}