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
	context->instrIndex++;

	for (auto& stmt : stmts) {
		auto err = stmt->compile(this->context);
		if (err) return err;
	}

	Output::code() << Exit{ };
	context->instrIndex++;
	return EXIT_SUCCESS;
}

int Return::compile(BlockContext context)
{
	auto err = expr.compile(context);
	if (err) return err;

	Output::code() << Ret{ };
	context->instrIndex++;
	return EXIT_SUCCESS;
}

int Branch::compile(BlockContext context)
{
	// put neagted condition on the stack
	auto err = cond.compile(context);
	if (err) return err;
	Output::code() << Not{ };
	context->instrIndex++;

	// buffer block body
	Output::Bytecode::push();
	err = body.compile(context);
	if (err) return err;
	auto buf = Output::Bytecode::pop();

	// put jump target for body skip on the stack
	buf->seekg(std::ios::end);
	size_t bodyLen = buf->tellg();
	buf->seekg(std::ios::beg);
	auto jmpTarget = (operand) (context->instrIndex + bodyLen + 1);
	Output::code() << Push{ jmpTarget };
	context->instrIndex++;

	// output Jmp instruction for body skip
	Output::code() << Jmp{ };
	context->instrIndex++;

	// output body bytecode
	Output::code() << buf->rdbuf();
	context->instrIndex += bodyLen;

	return EXIT_SUCCESS;
}

int Loop::compile(BlockContext context)
{
	// remember loop entry point
	auto loopBegin = (operand) context->instrIndex;

	// put negated condition on the stack
	auto err = cond.compile(context);
	if (err) return err;
	Output::code() << Not{ };
	context->instrIndex++;

	// buffer block body
	Output::Bytecode::push();
	err = body.compile(context);
	if (err) return err;
	auto buf = Output::Bytecode::pop();

	// put jump target for body skip on the stack
	buf->seekg(std::ios::end);
	size_t bodyLen = buf->tellg();
	buf->seekg(std::ios::beg);
	auto jmpTarget = (operand)(context->instrIndex + bodyLen + 4);
	Output::code() << Push{ jmpTarget };
	context->instrIndex++;

	// output Jmp instruction for body skip
	Output::code() << Jmp{ };
	context->instrIndex++;

	// output body bytecode
	Output::code() << buf->rdbuf();
	context->instrIndex += bodyLen;

	// unconditionally jump to the beginning of the body
	Output::code() << Push{ 1 } << Push{ loopBegin } << Jmp{};
	context->instrIndex += 3;

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
	// check that main is defined
	bool hasMain = false;
	for (auto& f : functions) {
		if (f.name == "main") {
			hasMain = true;
			break;
		}
	}
	if (!hasMain) {
		Output::error() << "main() is not defined.\n";
		return EXIT_FAILURE;
	}

	// compile functions
	instrIndex = functions.size();
	for (auto& f : functions) {
		f.context->instrIndex = instrIndex;
		auto err = f.compile(f.context);
		if (err) return err;
		instrIndex = f.context->instrIndex;
	}

	// TODO: compile function table

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
