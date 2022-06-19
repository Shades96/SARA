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
    char op;
    while (in.good()) {
        in.read(&op, 1);
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


// TODO
void Neg::parse(std::istream& in) {}
void Add::parse(std::istream& in) {}
void Sub::parse(std::istream& in) {}
void Mul::parse(std::istream& in) {}
void Div::parse(std::istream& in) {}
void Mod::parse(std::istream& in) {}
void Gt::parse(std::istream& in) {}
void Lt::parse(std::istream& in) {}
void Eq::parse(std::istream& in) {}
void Neq::parse(std::istream& in) {}
void Geq::parse(std::istream& in) {}
void Leq::parse(std::istream& in) {}
void Not::parse(std::istream& in) {}
void And::parse(std::istream& in) {}
void Or::parse(std::istream& in) {}
void Pop::parse(std::istream& in) {}
void Push::parse(std::istream& in) {}
void Enter::parse(std::istream& in) {}
void Exit::parse(std::istream& in) {}
void Kill::parse(std::istream& in) {}
void Call::parse(std::istream& in) {}
void Ret::parse(std::istream& in) {}
void Jmp::parse(std::istream& in) {}
