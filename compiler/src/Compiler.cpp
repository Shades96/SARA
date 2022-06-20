#include "Compiler.h"

int Expression::compile()
{
	return EXIT_SUCCESS;
}

int LExpression::compile()
{
	return EXIT_SUCCESS;
}

int FunctionCall::compile()
{
	return EXIT_SUCCESS;
}

int Term::compile()
{
	return EXIT_SUCCESS;
}

int Block::compile()
{
	return EXIT_SUCCESS;
}

int Return::compile()
{
	return EXIT_SUCCESS;
}

int Branch::compile()
{
	return EXIT_SUCCESS;
}

int Loop::compile()
{
	return EXIT_SUCCESS;
}

int Definition::compile()
{
	return EXIT_SUCCESS;
}

int Assignment::compile()
{
	return EXIT_SUCCESS;
}

int Function::compile()
{
	return EXIT_SUCCESS;
}

int Program::compile()
{
	return EXIT_SUCCESS;
}

int BracketPair::compile()
{
	return EXIT_SUCCESS;
}

int ParameterList::compile()
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
