#pragma once

#include "Instruction.h"

word fib(word n) {
	word curr = 1, last = 1;
	word i = 2, tmp;
	while (i < n) {
		tmp = curr;
		curr += last;
		last = tmp;
		i++;
	}
	return curr;
}