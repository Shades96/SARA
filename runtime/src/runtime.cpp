#include "Runtime.h"
#include "Output.h"
#include "Instruction.h"

vector<std::shared_ptr<Instruction>> Instruction::fromBytecode(std::istream& in) 
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
        case Instruction::Opcode::EXIT:
            result.push_back(std::make_shared<Exit>());
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
void Neg::exec() {}
void Add::exec() {}
void Sub::exec() {}
void Mul::exec() {}
void Div::exec() {}
void Mod::exec() {}
void Gt::exec() {}
void Lt::exec() {}
void Eq::exec() {}
void Neq::exec() {}
void Geq::exec() {}
void Leq::exec() {}
void Not::exec() {}
void And::exec() {}
void Or::exec() {}
void Pop::exec() {}
void Push::exec() {}
void Exit::exec() {}
void Call::exec() {}
void Ret::exec() {}
void Jmp::exec() {}

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
void Exit::parse(std::istream& in) {}
void Call::parse(std::istream& in) {}
void Ret::parse(std::istream& in) {}
void Jmp::parse(std::istream& in) {}
