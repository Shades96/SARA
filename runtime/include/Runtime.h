#pragma once

#include "Instruction.h"

class Runtime
{
	ExecContext context;
	ExecStatus status = ExecStatus::SUCCESS;
public:
	int run();
};