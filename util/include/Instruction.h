#pragma once

#include <vector>
#include <string>

using std::vector, std::string;

struct Instruction 
{
	enum Opcode 
	{
        // Arithmetic
        Neg,    // <op1>
        Add,    // <op1> <op2>
        Sub,    // <op1> <op2>
        Mul,    // <op1> <op2>
        Div,    // <op1> <op2>
        Mod,    // <op1> <op2>

        // Comparisons
        Gt,     // <op1> <op2>
        Lt,     // <op1> <op2>
        Eq,     // <op1> <op2>
        Neq,    // <op1> <op2>
        Geq,    // <op1> <op2>
        Leq,    // <op1> <op2>

        // Boolean
        Not,    // <op1>
        And,    // <op1> <op2>
        Or,     // <op1> <op2>

        // Stack
        Pop,    // <op1>
        Push,   // <op1>

        // Control
        Exit,   // 
        Call,   // <op1>
        Ret,    // <op1>
        Jmp,    // <op1> <op2>
	} op;
	vector<int> operands;
    Instruction(Opcode op) : op(op) {}
};

struct Add : public Instruction
{
    Add(Instruction::Opcode op) : Instruction(op) {}
};