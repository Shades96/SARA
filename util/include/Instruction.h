#pragma once

#include <vector>
#include <iostream>
#include <memory>

using std::vector;

struct Instruction;
using instr_seq = vector<std::shared_ptr<Instruction>>;
using instr_ptr = instr_seq::size_type;
using word = long;

struct ExecContext
{
    instr_seq	 instrs;
    instr_ptr	 ip;
    vector<word> stack;
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
        NEG,    // <op1>
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
        POP,    // <op1>
        PUSH,   // <op1>

        // Control
        EXIT,   // 
        CALL,   // <op1>
        RET,    // <op1>
        JMP,    // <op1> <op2>
	} op;

	vector<int> operands;
    virtual ExecStatus exec(ExecContext &context) = 0;
    virtual void parse(std::istream& in) = 0;

    Instruction(Opcode op) : op(op) {}
    static vector<std::shared_ptr<Instruction>> fromBytecode(std::istream& in);
};

struct Neg : public Instruction
{
    Neg() : Instruction(Opcode::NEG) {}
	ExecStatus exec(ExecContext &context) override;
    void parse(std::istream& in) override;
};
struct Add : public Instruction
{
    Add() : Instruction(Opcode::ADD) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Sub : public Instruction
{
    Sub() : Instruction(Opcode::SUB) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Mul : public Instruction
{
    Mul() : Instruction(Opcode::MUL) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Div : public Instruction
{
    Div() : Instruction(Opcode::DIV) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Mod : public Instruction
{
    Mod() : Instruction(Opcode::MOD) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Gt : public Instruction
{
    Gt() : Instruction(Opcode::GT) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Lt : public Instruction
{
    Lt() : Instruction(Opcode::LT) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Eq : public Instruction
{
    Eq() : Instruction(Opcode::EQ) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Neq : public Instruction
{
    Neq() : Instruction(Opcode::NEQ) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Geq : public Instruction
{
    Geq() : Instruction(Opcode::GEQ) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Leq : public Instruction
{
    Leq() : Instruction(Opcode::LEQ) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Not : public Instruction
{
    Not() : Instruction(Opcode::NOT) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct And : public Instruction
{
    And() : Instruction(Opcode::AND) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Or : public Instruction
{
    Or() : Instruction(Opcode::OR) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Pop : public Instruction
{
    Pop() : Instruction(Opcode::POP) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Push : public Instruction
{
    Push() : Instruction(Opcode::PUSH) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Exit : public Instruction
{
    Exit() : Instruction(Opcode::EXIT) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Call : public Instruction
{
    Call() : Instruction(Opcode::CALL) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Ret : public Instruction
{
    Ret() : Instruction(Opcode::RET) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};
struct Jmp : public Instruction
{
    Jmp() : Instruction(Opcode::JMP) {}
	ExecStatus exec(ExecContext &context) override;
	void parse(std::istream& in) override;
};

