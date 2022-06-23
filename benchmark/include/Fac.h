#pragma once

#include "Instruction.h"

word fac(word n) {
	word i = 1, result = 1;
	while (i < n) {
		i++;
		result *= i;
	}
	return result;
}