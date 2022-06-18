#pragma once

#include <vector>
#include <iostream>
#include <memory>

using std::vector;

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
    static vector<std::shared_ptr<Instruction>> fromBytecode(std::istream& in);
    virtual void exec() = 0;
    virtual void parse(std::istream& in) = 0;
    Instruction(Opcode op) : op(op) {}
};

struct Neg : public Instruction
{
    Neg() : Instruction(Opcode::NEG) {}
	void exec() override;
    void parse(std::istream& in) override;
};
struct Add : public Instruction
{
    Add() : Instruction(Opcode::ADD) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Sub : public Instruction
{
    Sub() : Instruction(Opcode::SUB) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Mul : public Instruction
{
    Mul() : Instruction(Opcode::MUL) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Div : public Instruction
{
    Div() : Instruction(Opcode::DIV) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Mod : public Instruction
{
    Mod() : Instruction(Opcode::MOD) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Gt : public Instruction
{
    Gt() : Instruction(Opcode::GT) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Lt : public Instruction
{
    Lt() : Instruction(Opcode::LT) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Eq : public Instruction
{
    Eq() : Instruction(Opcode::EQ) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Neq : public Instruction
{
    Neq() : Instruction(Opcode::NEQ) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Geq : public Instruction
{
    Geq() : Instruction(Opcode::GEQ) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Leq : public Instruction
{
    Leq() : Instruction(Opcode::LEQ) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Not : public Instruction
{
    Not() : Instruction(Opcode::NOT) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct And : public Instruction
{
    And() : Instruction(Opcode::AND) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Or : public Instruction
{
    Or() : Instruction(Opcode::OR) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Pop : public Instruction
{
    Pop() : Instruction(Opcode::POP) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Push : public Instruction
{
    Push() : Instruction(Opcode::PUSH) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Exit : public Instruction
{
    Exit() : Instruction(Opcode::EXIT) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Call : public Instruction
{
    Call() : Instruction(Opcode::CALL) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Ret : public Instruction
{
    Ret() : Instruction(Opcode::RET) {}
	void exec() override;
	void parse(std::istream& in) override;
};
struct Jmp : public Instruction
{
    Jmp() : Instruction(Opcode::JMP) {}
	void exec() override;
	void parse(std::istream& in) override;
};

