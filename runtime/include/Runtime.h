#pragma once

#include "Instruction.h"

class Runtime
{
	ExecContext context;
	ExecStatus status = ExecStatus::SUCCESS;
public:
	Runtime(instr_seq instructions) : context(instructions) {}
	int run();
};