#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

using std::vector, std::string;

struct Instruction;
using word = long long;             // 64 bit
using operation = unsigned char;    //  8 bit
using operand = short;     // 16 bit
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
    vector<stack_ptr>   fsfp;       // function stack frame pointer
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
        NEG = 0,    // <op1>
        ADD = 1,    // <op1> <op2>
        SUB = 2,    // <op1> <op2>
        MUL = 3,    // <op1> <op2>
        DIV = 4,    // <op1> <op2>
        MOD = 5,    // <op1> <op2>

        // Comparisons
        GT = 6,     // <op1> <op2>
        LT = 7,     // <op1> <op2>
        EQ = 8,     // <op1> <op2>
        NEQ = 9,    // <op1> <op2>
        GEQ = 10,   // <op1> <op2>
        LEQ = 11,   // <op1> <op2>

        // Boolean
        NOT = 12,   // <op1>
        AND = 13,   // <op1> <op2>
        OR = 14,    // <op1> <op2>

        // Stack
        POP = 15,   // <src> <dest>
        PUSH = 16,  // <VAL>
        LOAD = 17,  // <src>

        // Flow control
        ENTR = 18,  //
        EXIT = 19,  //
        KILL = 20,  //
        CALL = 21,  //
        RET = 22,   // <op1>
        JMP = 23,   // <op1>
        JMPC = 24,  // <op1> <op2>

        // Builtin functions
        PRNT = 25,  // <op1>
        READ = 26   //
	} op;

    size_t numStackOperands = 0;
    size_t numConstOperands = 0;
    vector<operand> constOperands;
    vector<word> stackOperands;
    virtual ExecStatus exec(ExecContext &context);

    static instr_seq fromBytecode(std::istream& in);

    void parse(std::istream& in)
    {
        // parse operands
        for (int i = 0; i < numConstOperands; i++) {
            OperandConversion converted;
            in.read(&converted.bytes.b1, 1);
            in.read(&converted.bytes.b2, 1);
            constOperands.push_back(converted.o);
        }
    }

    string to_string() const
    {
        return INSTR_NAMES[op];
    }

    Instruction(Opcode op) : op(op) {}
    Instruction(Opcode op, size_t numStackOperands) : op(op), 
        numStackOperands(numStackOperands), numConstOperands(0) {}
    Instruction(Opcode op, size_t numStackOperands, size_t numConstOperands) : op(op), 
        numStackOperands(numStackOperands), numConstOperands(numConstOperands) {}
    Instruction(Opcode op, size_t numStackOperands, size_t numConstOperands, vector<operand> constOperands) : op(op),
        numStackOperands(numStackOperands), numConstOperands(numConstOperands), constOperands(constOperands) {}
    vector<char> toBytes() const;
    static const inline vector<string> INSTR_NAMES {
            "NEG",            "ADD",            "SUB",            "MUL",            "DIV",            "MOD",            "GT",            "LT",            "EQ",            "NEQ",            "GEQ",            "LEQ",            "NOT",            "AND",            "OR",            "POP",            "PUSH",            "LOAD",            "ENTR",            "EXIT",            "KILL",            "CALL",            "RET",            "JMP",            "JMPC",            "PRNT",            "READ",
    };
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
    Pop() : Instruction(Opcode::POP, 2) {}
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
    Call() : Instruction(Opcode::CALL, 0) {}    // TODO: no operand
	ExecStatus exec(ExecContext &context) override;
};
struct Jmp : public Instruction
{
    Jmp() : Instruction(Opcode::JMP, 1) {}
    ExecStatus exec(ExecContext& context) override;
};
struct Jmpc : public Instruction
{
    Jmpc() : Instruction(Opcode::JMPC, 2) {}
	ExecStatus exec(ExecContext &context) override;
};
struct Print : public Instruction
{
    Print() : Instruction(Opcode::PRNT, 1) {}
    ExecStatus exec(ExecContext& context) override;
};
struct Read : public Instruction
{
    Read() : Instruction(Opcode::READ, 0) {}
    ExecStatus exec(ExecContext& context) override;
};
