#include "Runtime.h"
#include "Output.h"

int Runtime::run()
{
    while (status == ExecStatus::SUCCESS) {
        status = context.instrs[context.ip]->exec(context);
    }
    return status;
}

instr_seq Instruction::fromBytecode(std::istream& in) 
{
    vector<std::shared_ptr<Instruction>> result;
    operation op;
    char tmp;
    while (in.read(&tmp, 1).good()) {
        // parse opcode
        op = (operation) tmp;
        auto opcode = (Instruction::Opcode) op;
        switch (opcode)
        {
        case Instruction::Opcode::NEG:
            result.push_back(std::make_shared<Neg>());
            break;
        case Instruction::Opcode::ADD:
            result.push_back(std::make_shared<Add>());
            break;
        case Instruction::Opcode::SUB:
            result.push_back(std::make_shared<Sub>());
            break;
        case Instruction::Opcode::MUL:
            result.push_back(std::make_shared<Mul>());
            break;
        case Instruction::Opcode::DIV:
            result.push_back(std::make_shared<Div>());
            break;
        case Instruction::Opcode::MOD:
            result.push_back(std::make_shared<Mod>());
            break;
        case Instruction::Opcode::GT:
            result.push_back(std::make_shared<Gt>());
            break;
        case Instruction::Opcode::LT:
            result.push_back(std::make_shared<Lt>());
            break;
        case Instruction::Opcode::EQ:
            result.push_back(std::make_shared<Eq>());
            break;
        case Instruction::Opcode::NEQ:
            result.push_back(std::make_shared<Neq>());
            break;
        case Instruction::Opcode::GEQ:
            result.push_back(std::make_shared<Geq>());
            break;
        case Instruction::Opcode::LEQ:
            result.push_back(std::make_shared<Leq>());
            break;
        case Instruction::Opcode::NOT:
            result.push_back(std::make_shared<Not>());
            break;
        case Instruction::Opcode::AND:
            result.push_back(std::make_shared<And>());
            break;
        case Instruction::Opcode::OR:
            result.push_back(std::make_shared<Or>());
            break;
        case Instruction::Opcode::POP:
            result.push_back(std::make_shared<Pop>());
            break;
        case Instruction::Opcode::PUSH:
            result.push_back(std::make_shared<Push>());
            break;
        case Instruction::Opcode::ENTR:
            result.push_back(std::make_shared<Enter>());
            break;
        case Instruction::Opcode::EXIT:
            result.push_back(std::make_shared<Exit>());
            break;
        case Instruction::Opcode::KILL:
            result.push_back(std::make_shared<Kill>());
            break;
        case Instruction::Opcode::CALL:
            result.push_back(std::make_shared<Call>());
            break;
        case Instruction::Opcode::RET:
            result.push_back(std::make_shared<Ret>());
            break;
        case Instruction::Opcode::JMP:
            result.push_back(std::make_shared<Jmp>());
            break;
        default:
            Output::error() << "Unknown istruction (" << std::to_string(opcode) << ") " 
                << "at position " << std::to_string(result.size() + 1) << "\n";
            return result;
            break;
        }
        result.back()->parse(in);
    }
    return result;
}

void Instruction::parse(std::istream& in) {
    // parse operands
    for (int i = 0; i < numConstOperands; i++) {
        OperandConversion converted;
        in.read(&converted.bytes.b1, 1);
        in.read(&converted.bytes.b2, 1);
        operands.push_back(converted.o);
    }
}

// TODO
ExecStatus Neg::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Add::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Sub::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Mul::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Div::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Mod::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Gt::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Lt::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Eq::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Neq::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Geq::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Leq::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Not::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus And::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Or::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Pop::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Push::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Load::exec(ExecContext& context) {
    return ExecStatus::FAIL;
}

ExecStatus Enter::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Exit::exec(ExecContext& context) {
    return ExecStatus::FAIL;
}

ExecStatus Kill::exec(ExecContext& context) {
    return ExecStatus::FAIL;
}

ExecStatus Call::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Ret::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}

ExecStatus Jmp::exec(ExecContext &context) {
	return ExecStatus::FAIL;
}
