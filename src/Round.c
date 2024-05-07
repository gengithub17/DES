#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Constant.h"
#include "Values.h"

int Init_SBox(int**** sbox){
	int*** sboxaddr = (int***)malloc(SBOXNUM*sizeof(int**));
	for(int i=0; i<SBOXNUM; i++){
		sboxaddr[i] = (int**)malloc(SBOXROW*sizeof(int*));
		for(int j=0; j<SBOXROW; j++){
			sboxaddr[i][j] = (int*)malloc(SBOXCOL*sizeof(int));
			for(int k=0; k<SBOXCOL; k++){
				sboxaddr[i][j][k] = SBOX[i][j][k];
			}
		}
	}
	memcpy(sbox, &sboxaddr, sizeof(int****));
	return 0;
}

int getval_SBox(int** sbox, int inputnum, int* outputnum){
	int row, col;
	row = ((inputnum&0b100000)>>4)||(inputnum&0b000001);
	col = (inputnum&0b011110)>>1;
	*outputnum = sbox[row][col];
	return 0;
}

int f(int r, long k, unsigned* ret, int*** sbox){
	long extended = 0;
	// extend
	for(int i=0; i<48; i++){
		extended <<= 1;
		if(r&(1U<<E[i])) extended |= 1;
	}
#ifdef DEBUG_ROUND_F
	printbinary("extended before = \t",extended, 48);
#endif /*DEBUG_ROUND_F*/
	extended ^= k;
#ifdef DEBUG_ROUND_F
	printbinary("extended after = \t",extended, 48);
#endif /*DEBUG_ROUND_F*/
	// sbox
	int bit6, sbox_val;
	unsigned bit32 = 0;
	for(int i=0; i<SBOXNUM; i++){
		bit32 <<= 4;
		bit6 = (extended>>(6*(SBOXNUM-1-i)))&0b111111;
		getval_SBox(sbox[i], bit6, &sbox_val);
		bit32 += sbox_val;
#ifdef DEBUG_ROUND_F
		printf("%dth SBox\n",i);
		printbinary("\tbit6 = \t",(long)bit6, 6);
		printbinary("\tbit4 = \t", (long)sbox_val,4);
#endif /*DEBUG_ROUND_F*/
	}
#ifdef DEBUG_ROUND_F
	printbinary("after sbox = \t\t",bit32, 32);
#endif /*DEBUG_ROUND_F*/
	// trans
	*ret = 0;
	for(int i=0; i<32; i++){
		*ret <<= 1;
		if(bit32&(1U<<P[i])) *ret |= 1;
	}
#ifdef DEBUG_ROUND_F
	printbinary("\tf(r,k) = \t",(long)*ret,32);
	printf("\n");
#endif /*DEBUG_ROUND_F*/
	return 0;
}

int Round(unsigned l, unsigned r, long k, unsigned* newl, unsigned* newr, int*** sbox){
#ifdef DEBUG_ROUND
	printf("Round :\n");
	printbinary("\tl = \t\t",(long)l,32);
	printbinary("\tr = \t\t",(long)r,32);
#ifndef DEBUG_KEY
	printbinary("\tk = \t\t", k, 48);
#endif /*not DEBUG_KEY*/
#endif /*DEBUG_ROUND*/
	*newl = r;
	unsigned fret;
	f(r, k, &fret, sbox);
	*newr = l^fret;
#ifdef DEBUG_ROUND
#ifndef DEBUG_ROUND_F
	printbinary("\tf(r,k) = \t",(long)fret,32);
#endif/*not DEBUG_ROUND_F*/
	printbinary("\tnew l = \t",(long)*newl,32);
	printbinary("\tnew r = \t",(long)*newr,32);
	printf("\n");
#endif /*DEBUG_ROUND*/
	return 0;
}

#ifdef TEST_MODE
int Round_Test(unsigned l, unsigned r, long k, int*** sbox){
#ifndef DEBUG_ROUND
	printf("It is strongly recommended to enable DEBUG_ROUND for detail values!");
#endif /*DEBUG_ROUND*/
	unsigned newl, newr;
	Round(l,r,k,&newl,&newr,sbox);
	unsigned retl, retr;
	Round(newr,newl,k,&retr,&retl,sbox);
	if(retl==l && retr==r){
		printf("Success : Applying the 'Round' function twice will return to the original value\n");
		return TEST_SUCCESS;
	}else{
		printf("Fail : The original values were not returned.\n");
		return TEST_FAIL;
	}
}
#endif /*TEST_MODE*/