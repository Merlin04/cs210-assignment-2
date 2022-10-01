#include <stdio.h>
#include <stdlib.h>
#include "../libcompact.h"

/*
version 1
int conditional(int x, int y, int z) {
	I16 mask = (x != 0) * U16_MAX;
	return (mask & y) + (~mask & z);
}
*/

// version 2
int conditional(int x, int y, int z) {
	return (x != 0) * y | (x == 0) * z;
}


// Test
MAKE_TEST_FN_STRUCT(1,conditional,Pd,I,I x;I y;I z;,(a.x,a.y,a.z),4,{
	{50,1,2}, // 1
	{0,5,8}, // 8
	{-8,3,-2}, // 3
	{-1000, -100, -800} // -100
})
int main(){RUN_FNS(t1)}
