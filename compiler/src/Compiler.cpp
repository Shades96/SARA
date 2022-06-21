#include "Compiler.h"

int Expression::compile(BlockContext context)
{
	auto err = term1->compile(context);
	if (err) return err;

	if (term2) {
		err = term2->compile(context);
		if (err) return err;
	}

	switch (kind) {
	case Kind::TERM:
		break;
	case Kind::NEGATION:
		Output::code() << Not{ };
		break;
	case Kind::NEGATIVE:
		Output::code() << Neg{ };
		break;
	case Kind::ADDITION:
		Output::code() << Add{ };
		break;
	case Kind::SUBTRACTION:
		Output::code() << Sub{ };
		break;
	case Kind::MULTIPLICATION:
		Output::code() << Mul{ };
		break;
	case Kind::DIVISION:
		Output::code() << Div{ };
		break;
	case Kind::MODULO:
		Output::code() << Mod{ };
		break;
	case Kind::CONJUNCTION:
		Output::code() << And{ };
		break;
	case Kind::DISJUNCTION:
		Output::code() << Or{ };
		break;
	case Kind::COMPARISON_LT:
		Output::code() << Lt{ };
		break;
	case Kind::COMPARISON_EQ:
		Output::code() << Eq{ };
		break;
	case Kind::COMPARISON_GT:
		Output::code() << Gt{ };
		break;
	default:
		Output::error() << "Unlabeled expression\n";
		return EXIT_FAILURE;
		break;
	}
	context->instrIndex++;
	return EXIT_SUCCESS;
}

int LExpression::compile(BlockContext context)
{
	switch (kind) {
	case VAR_WRITE:
		Output::code() << Push{ (operand) context->variables[id.id].stackLocation } << Pop{ };
		context->instrIndex++;
		break;
	case ARRAY_WRITE:
		// TODO
		break;
	default:
		Output::error() << "Unlabeled L-expression\n";
		return EXIT_FAILURE;
		break;
	}
	return EXIT_SUCCESS;
}

int FunctionCall::compile(BlockContext context)
{
	// compile parameter expressions
	for (auto& expr : params) {
		auto err = expr.compile(context);
		if (err) return err;
	}

	// handle builtin functions
	if (funName == "kill") {
		Output::code() << Kill{ };
		context->instrIndex++;
	}
	else if (funName == "print") {
		Output::code() << Print{ };
		context->instrIndex++;
	}
	else if (funName == "read") {
		Output::code() << Read{ };
		context->instrIndex++;
	}

	// TODO: handle user defined functions

	return EXIT_SUCCESS;
}

int Term::compile(BlockContext context)
{
	switch (kind) {
	case Kind::NUMBER:
		Output::code() << Push{ (operand) id.value };
		context->instrIndex++;
		break;
	case Kind::REFERENCE:
		Output::code() << Push{ (operand) context->variables[id.id].stackLocation } << Load{ };
		context->instrIndex += 2;
		break;
	case Kind::ARRAY_READ:
		// TODO
		break;
	case Kind::FUNCTION_CALL:
		// TODO
		break;
	case Kind::EXPRESSION:
		return expr->compile(context);
		break;
	default:
		Output::error() << "Unlabeled term\n";
		return EXIT_FAILURE;
		break;
	}
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
	return expr.compile(context);
}

int Assignment::compile(BlockContext context)
{
	// compile RExpression
	auto err = expr.compile(context);
	if (err) return err;

	// compile LExpression
	return lexpr.compile(context);
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
	for (auto o : constOperands) {
		OperandConversion converted;
		converted.o = o;
		result.push_back((char) converted.bytes.b1);
		result.push_back((char) converted.bytes.b2);
	}
	return result;
}
