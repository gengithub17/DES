#include <stdio.h>
#include <stdlib.h>
#include "Values.h"

int Init_Key(long k, int* c, int* d){
	long transk = 0;
	for(int i=0; i<56; i++){
		transk <<= 1;
		if(k&(1L<<PC_1[i])) transk |= 1;
	}
	*c = (int)(transk>>28);
	*d = (int)(transk&0xFFFFFFF);
	return 0;
}

int Rotate_Shift(int val, int len, int shift, int* ret){
	int lsb;
	lsb = val>>(len-shift);
	*ret = ((val<<shift)|lsb) & ((1<<len)-1);
	return 0;
}

int Key_Manage(int round, int c, int d, long* newk, int* newc, int* newd){
	Rotate_Shift(c, 28, KeyShift[round], newc);
	Rotate_Shift(d, 28, KeyShift[round], newd);
	// trans
	long bit56 = ((long)*newc)<<28 | (long)*newd;
	*newk = 0;
	for(int i=0; i<48; i++){
		*newk <<= 1;
		if(bit56&(1L<<PC_2[i])) *newk |= 1;
	}
#ifdef DEBUG_KEY
	printf("EncKey round%d\nshift:%d\n",round, KeyShift[round]);
	printbinary("new C = \t", (long)*newc, 28);
	printbinary("new D = \t", (long)*newd, 28);
	printbinary("bit56 = \t", bit56, 56);
	printbinary("key = \t", *newk, 48);
	printf("\n");
#endif /*DEBUG_KEY*/
	return 0;
}

int Key_invManage(int round, int c, int d, long* newk, int* newc, int* newd){
	long bit56 = ((long)c<<28) | (long)d;
	*newk = 0;
	for(int i=0; i<48; i++){
		*newk <<= 1;
		if(bit56&(1L<<PC_2[i])) *newk |= 1;
	}
	Rotate_Shift(c, 28, 28-KeyShift[round], newc);
	Rotate_Shift(d, 28, 28-KeyShift[round], newd);
#ifdef DEBUG_KEY
	printf("DecKey round%d\nshift:%d\n",round, KeyShift[round]);
	printbinary("C = \t", (long)c, 28);
	printbinary("D = \t", (long)*newd, 28);
	printbinary("bit56 = \t", bit56, 56);
	printbinary("key = \t", *newk, 48);
	printf("\n");
#endif /*DEBUG_KEY*/
	return 0;
}