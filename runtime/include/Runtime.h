#pragma once

#include "Instruction.h"

class Runtime
{
public:
	ExecContext context;
	ExecStatus status = ExecStatus::SUCCESS;
	Runtime(instr_seq instructions) : context(instructions) {}
	int run();
};