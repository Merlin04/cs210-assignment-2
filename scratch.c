// This doesn't meet requirements because it uses loops
/*int conditional(int x, int y, int z) {
	U16 mask = 0;
	for(U8 i = 0; i < 16; i++) {
		U8 t = (x << (15 - i)) >> 15;
		for(U8 j = 0; j < 16; j++) {
			mask |= t;
			t <<= 1;
		}
	}
	P("%d,%d,%d\n",x,y,z);
	Pd(mask);
	Pb(mask);
	return (mask & y) + (~mask & z);
}*/
