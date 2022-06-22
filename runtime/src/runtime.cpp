#include "Runtime.h"
#include "Output.h"

int Runtime::run()
{
    while (status == ExecStatus::SUCCESS) {
        auto fetched = context.instrs[context.ip];
        status = fetched->Instruction::exec(context);
        status = fetched->exec(context);
    }
    return status;
}


ExecStatus Instruction::exec(ExecContext& context)
{
    // pop stack operands
    stackOperands.clear();
    for (int i = 0; i < numStackOperands; i++) {
        stackOperands.push_back(context.stack.back());
        context.stack.pop_back();
    }
    // advance instruction pointer
    context.ip++;
    return ExecStatus::SUCCESS;
}

ExecStatus Neg::exec(ExecContext &context)
{
    context.stack.push_back(-stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Add::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] + stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Sub::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] - stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Mul::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] * stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Div::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] / stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Mod::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] % stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Gt::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] > stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Lt::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] < stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Eq::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] == stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Neq::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] != stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Geq::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] >= stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Leq::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] <= stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Not::exec(ExecContext &context)
{
    context.stack.push_back(!stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus And::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] && stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Or::exec(ExecContext &context)
{
    context.stack.push_back(stackOperands[1] || stackOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Pop::exec(ExecContext &context)
{
    context.stack[stackOperands[0]] = stackOperands[1];
	return ExecStatus::SUCCESS;
}

ExecStatus Push::exec(ExecContext &context)
{
    context.stack.push_back(constOperands[0]);
	return ExecStatus::SUCCESS;
}

ExecStatus Load::exec(ExecContext& context)
{
    context.stack.push_back(context.stack[stackOperands[0] + context.fsfp.back()]);
    return ExecStatus::SUCCESS;
}

ExecStatus Enter::exec(ExecContext &context)
{
    // TODO
	return ExecStatus::SUCCESS;
}

ExecStatus Exit::exec(ExecContext& context)
{
    // TODO
    return ExecStatus::SUCCESS;
}

ExecStatus Kill::exec(ExecContext& context)
{
    return ExecStatus::EXIT;
}

ExecStatus Call::exec(ExecContext &context)
{
    context.fsfp.push_back(context.stack.size() - 1);   // save fsfp
	return ExecStatus::SUCCESS;
}

ExecStatus Ret::exec(ExecContext &context)
{
    // cleanup everything past last fsfp, then pop it
    while (context.stack.size() > context.fsfp.back() + 1) {
        context.stack.pop_back();
    }
    context.fsfp.pop_back();

    // jump back to saved return address
	return ExecStatus::SUCCESS;
}

ExecStatus Jmp::exec(ExecContext &context)
{
    auto cond = stackOperands[1];
    if (cond) {
        context.ip = stackOperands[0];
    }
	return ExecStatus::SUCCESS;
}

ExecStatus Print::exec(ExecContext& context)
{
    Output::print() << std::to_string(stackOperands[0]);
    return ExecStatus::SUCCESS;
}

ExecStatus Read::exec(ExecContext& context)
{
    // TODO
    return ExecStatus::SUCCESS;
}

instr_seq Instruction::fromBytecode(std::istream& in)
{
    vector<std::shared_ptr<Instruction>> result;
    operation op;
    char tmp;
    while (in.read(&tmp, 1).good()) {
        // parse opcode
        op = (operation)tmp;
        auto opcode = (Instruction::Opcode)op;
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
        case Instruction::Opcode::LOAD:
            result.push_back(std::make_shared<Load>());
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
        case Instruction::Opcode::PRNT:
            result.push_back(std::make_shared<Print>());
            break;
        case Instruction::Opcode::READ:
            result.push_back(std::make_shared<Read>());
            break;
        default:
            //Output::error() << "Unknown istruction (" << std::to_string(opcode) << ") "
            //    << "at position " << std::to_string(result.size() + 1) << "\n";
            std::cout << "Unknown istruction (" << std::to_string(opcode) << ") "
                << "at position " << std::to_string(result.size() + 1) << "\n";
            return result;
            break;
        }
        result.back()->parse(in);
    }
    return result;
}