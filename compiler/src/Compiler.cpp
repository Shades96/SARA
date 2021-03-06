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
		context->instrIndex--;
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
	case Kind::COMPARISON_NEQ:
		Output::code() << Neq{ };
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
		context->instrIndex += 2;
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
		return EXIT_SUCCESS;
	}
	else if (funName == "print") {
		Output::code() << Print{ };
		context->instrIndex++;
		return EXIT_SUCCESS;
	}
	else if (funName == "read") {
		Output::code() << Read{ };
		context->instrIndex++;
		return EXIT_SUCCESS;
	}

	// handle user defined functions
	// save fsfp and return address
	context->instrIndex += 5 + (params.size() * 2);
	Output::code() << Push{ (operand) context->instrIndex } << Call{ };

	// copy parameters to next stack frame
	for (int i = -((int)params.size() + 1); i < -1; i++) {
		Output::code() << Push{ (operand) i } << Load{ };
	}

	// find respective function table entry
	auto funTableEntry = context->functionRefs.find(funName);
	if (funTableEntry == context->functionRefs.end()) {
		Output::error() << "No function table entry for '" << funName << "'\n";
		return EXIT_FAILURE;
	}

	// jump to the function
	auto jmpTarget = funTableEntry->second * 3;
	Output::code() << Push{ 1 } << Push{ (operand) jmpTarget } << Jmpc{};

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
		return funCall->compile(context);
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
	this->context->instrIndex += context->instrIndex; // TODO ???

	Output::code() << Enter{ };
	this->context->instrIndex++;

	for (auto& stmt : stmts) {
		auto err = stmt->compile(this->context);
		if (err) return err;
	}

	Output::code() << Exit{ };
	this->context->instrIndex++;
	context->instrIndex = this->context->instrIndex;
	return EXIT_SUCCESS;
}

int Return::compile(BlockContext context)
{
	auto err = expr.compile(context);
	if (err) return err;

	Output::code() << Ret{ } << Jmp{ };
	context->instrIndex += 2;

	return EXIT_SUCCESS;
}

int Branch::compile(BlockContext context)
{
	// put negated condition on the stack
	auto err = cond.compile(context);
	if (err) return err;
	Output::code() << Not{ };

	// preliminary incr for following Push/Jmpc because body needs to know
	context->instrIndex += 3;

	// buffer block body
	Output::Bytecode::push();
	err = body.compile(context);
	if (err) return err;
	auto buf = Output::Bytecode::pop();

	// compile conditional body skip
	auto jmpTarget = (operand) context->instrIndex;
	Output::code() << Push{ jmpTarget } << Jmpc{ };

	// output body bytecode
	Output::code() << buf;

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

	// preliminary incr for following Push/Jmpc because body needs to know
	context->instrIndex += 3;

	// buffer block body
	Output::Bytecode::push();
	err = body.compile(context);
	if (err) return err;
	auto buf = Output::Bytecode::pop();

	// compile conditional body skip
	auto jmpTarget = (operand)(context->instrIndex + 3);
	Output::code() << Push{ jmpTarget } << Jmpc{ };

	// output body bytecode
	Output::code() << buf;

	// unconditionally jump to the beginning of the body
	Output::code() << Push{ 1 } << Push{ loopBegin } << Jmpc{};
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
	auto err = body->compile(this->context);
	this->context->instrIndex = body->context->instrIndex;
	return err;
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

	// make room for function table
	instrIndex = functions.size() * 3;
	Output::Bytecode::push();

	// buffer compiled functions, save entry points
	for (auto& f : functions) {
		f.entryPoint = instrIndex;
		f.context->instrIndex = instrIndex;
		f.context->functionRefs = *functionRefs;
		auto err = f.compile(f.context);
		if (err) return err;
		instrIndex = f.context->instrIndex;
		for (auto& funRef : f.context->functionRefs) {
			auto funName = funRef.first;
			if (functionRefs->find(funName) == functionRefs->end()) {
				(*functionRefs)[funName] = functionRefs->size();
			}
		}
	}
	auto codeBuf = Output::Bytecode::pop();

	// compile function table
	for (auto& f : functions) {
		if (f.name == "main") {
			Output::code() << Push{ 1 } << Push{ (operand) f.entryPoint } << Jmpc{ };
		}
	}
	for (auto& f : functions) {
		if (f.name != "main") {
			Output::code() << Push{ 1 } << Push{ (operand) f.entryPoint } << Jmpc{ };
		}
	}

	// output compiled functions
	Output::code() << codeBuf;
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
