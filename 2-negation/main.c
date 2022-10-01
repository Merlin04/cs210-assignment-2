#include <stdio.h>
#include <stdlib.h>
#include "../libcompact.h"

int logical_negation(int i) {
	return i == 0;
}

// Test
MAKE_TEST_FN(1,logical_negation,I,Pd,1, -5, 8, 0, 3)
int main(){RUN_FNS(t1)}
