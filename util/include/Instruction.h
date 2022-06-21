#pragma once

#include <vector>
#include <iostream>
#include <memory>

using std::vector;

struct Instruction;
using word = long;
using operation = unsigned char;
using operand = unsigned short;
using stack = vector<word>;
using instr_seq = vector<std::shared_ptr<Instruction>>;
using instr_ptr = instr_seq::size_type;
using stack_ptr = stack::size_type;

union OperandConversion {
    operand o;
    struct Bytes {
        char b1, b2;
    } bytes;
};

struct ExecContext
{
    instr_seq	        instrs;     // instruction sequence
    instr_ptr	        ip;         // instruction pointer
    stack               stack;      // program stack
    stack_ptr           fsfp;       // function stack frame pointer
    vector<stack_ptr>   bsfp;       // stack of block stack frame pointers

    ExecContext(instr_seq instrs) : instrs{ instrs }, ip{ 0 }, stack{ }, fsfp{ 0 }, bsfp{ 0 } {}
};

enum ExecStatus {
    FAIL = -1,
    SUCCESS = 0,
    EXIT = 1
};

struct Instruction 
{
	enum Opcode 
	{
        // Arithmetic
        NEG = 0,// <op1>
        ADD,    // <op1> <op2>
        SUB,    // <op1> <op2>
        MUL,    // <op1> <op2>
        DIV,    // <op1> <op2>
        MOD,    // <op1> <op2>

        // Comparisons
        GT,     // <op1> <op2>
        LT,     // <op1> <op2>
        EQ,     // <op1> <op2>
        NEQ,    // <op1> <op2>
        GEQ,    // <op1> <op2>
        LEQ,    // <op1> <op2>

        // Boolean
        NOT,    // <op1>
        AND,    // <op1> <op2>
        OR,     // <op1> <op2>

        // Stack
        POP,    //
        PUSH,   // <op1>
        LOAD,   // <op1>

        // Flow control
        ENTR,   //
        EXIT,   //
        KILL,   //
        CALL,   // <op1>
        RET,    // <op1>
        JMP,    // <op1> <op2>
	} op;

    size_t numStackOperands = 0;
    size_t numConstOperands = 0;
	vector<operand> operands;
    virtual ExecStatus exec(ExecContext &context) = 0;
    void parse(std::istream& in);

    Instruction(Opcode op) : op(op) {}
    Instruction(Opcode op, size_t numStackOperands) : op(op), 
        numStackOperands(numStackOperands), numConstOperands(0) {}
    Instruction(Opcode op, size_t numStackOperands, size_t numConstOperands) : op(op), 
        numStackOperands(numStackOperands), numConstOperands(numConstOperands) {}
    Instruction(Opcode op, size_t numStackOperands, size_t numConstOperands, vector<operand> operands) : op(op),
        numStackOperands(numStackOperands), numConstOperands(numConstOperands), operands(operands) {}
    vector<char> toBytes() const;
    static instr_seq fromBytecode(std::istream& in);
};

struct Neg : public Instruction
{
    Neg() : Instruction(Opcode::NEG, 1) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Add : public Instruction
{
    Add() : Instruction(Opcode::ADD, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Sub : public Instruction
{
    Sub() : Instruction(Opcode::SUB, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Mul : public Instruction
{
    Mul() : Instruction(Opcode::MUL, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Div : public Instruction
{
    Div() : Instruction(Opcode::DIV, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Mod : public Instruction
{
    Mod() : Instruction(Opcode::MOD, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Gt : public Instruction
{
    Gt() : Instruction(Opcode::GT, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Lt : public Instruction
{
    Lt() : Instruction(Opcode::LT, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Eq : public Instruction
{
    Eq() : Instruction(Opcode::EQ, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Neq : public Instruction
{
    Neq() : Instruction(Opcode::NEQ, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Geq : public Instruction
{
    Geq() : Instruction(Opcode::GEQ, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Leq : public Instruction
{
    Leq() : Instruction(Opcode::LEQ, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Not : public Instruction
{
    Not() : Instruction(Opcode::NOT, 1) {}
	ExecStatus exec(ExecContext &context) override;
};
struct And : public Instruction
{
    And() : Instruction(Opcode::AND, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Or : public Instruction
{
    Or() : Instruction(Opcode::OR, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Pop : public Instruction
{
    Pop() : Instruction(Opcode::POP, 1) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Push : public Instruction
{
    Push() : Instruction(Opcode::PUSH, 0, 1) {}
    Push(operand value) : Instruction(Opcode::PUSH, 0, 1, vector<operand>{ value }) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Load : public Instruction
{
    Load() : Instruction(Opcode::LOAD, 1) {}
    ExecStatus exec(ExecContext& context) override;
};
struct Enter : public Instruction
{
    Enter() : Instruction(Opcode::ENTR, 0) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Exit : public Instruction
{
    Exit() : Instruction(Opcode::EXIT, 0) {}
    ExecStatus exec(ExecContext& context) override;
};
struct Kill : public Instruction
{
    Kill() : Instruction(Opcode::KILL, 0) {}
    ExecStatus exec(ExecContext& context) override;
};
struct Ret : public Instruction
{
    Ret() : Instruction(Opcode::RET, 0) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Call : public Instruction
{
    Call() : Instruction(Opcode::CALL, 1) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Jmp : public Instruction
{
    Jmp() : Instruction(Opcode::JMP, 2) {}
	ExecStatus exec(ExecContext &context) override;
};

