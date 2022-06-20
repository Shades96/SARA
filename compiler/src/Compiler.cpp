#include "Compiler.h"

int Expression::compile(BlockContext context)
{
	return EXIT_SUCCESS;
}

int LExpression::compile(BlockContext context)
{
	return EXIT_SUCCESS;
}

int FunctionCall::compile(BlockContext context)
{
	return EXIT_SUCCESS;
}

int Term::compile(BlockContext context)
{
	return EXIT_SUCCESS;
}

int Block::compile(BlockContext context)
{
	Output::code() << Enter{ };
	for (auto& stmt : stmts) {
		auto err = stmt->compile(this->context);
		if (err) return err;
	}
	Output::code() << Exit{ };
	return EXIT_SUCCESS;
}

int Return::compile(BlockContext context)
{
	auto err = expr.compile(context);
	if (err) return err;
	Output::code() << Ret{ };
	return EXIT_SUCCESS;
}

int Branch::compile(BlockContext context)
{
	auto err = cond.compile(context);
	if (err) return err;
	Output::code() << Ret{ };
	return EXIT_SUCCESS;
}

int Loop::compile(BlockContext context)
{
	return EXIT_SUCCESS;
}

int Definition::compile(BlockContext context)
{
	return EXIT_SUCCESS;
}

int Assignment::compile(BlockContext context)
{
	return EXIT_SUCCESS;
}

int Function::compile(BlockContext context)
{
	return body->compile(this->context);
}

int Program::compile()
{
	for (auto& f : functions) {
		auto err = f.compile(f.context);
		if (err) return err;
	}
	return EXIT_SUCCESS;
}

int BracketPair::compile(BlockContext context)
{
	return EXIT_SUCCESS;
}

int ParameterList::compile(BlockContext context)
{
	return EXIT_SUCCESS;
}

int Compiler::compile(Program& program)
{
	return program.compile();
}

vector<char> Instruction::toBytes() const
{
	vector<char> result;
	auto operationByte = (operation) op;
	result.push_back((char) operationByte);
	for (auto o : operands) {
		OperandConversion converted;
		converted.o = o;
		result.push_back((char) converted.bytes.b1);
		result.push_back((char) converted.bytes.b2);
	}
	return result;
}
