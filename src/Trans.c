#include <stdio.h>
#include <stdlib.h>
#include "Values.h"

int Trans_IP(unsigned long m, unsigned* l, unsigned* r){
	unsigned long transm = 0;
	for(int i=0; i<64; i++){
		transm <<= 1;
		if(m&(1L<<IP[i])) transm |= 1;
	}
	*l = (unsigned)(transm>>32);
	*r = (unsigned)transm&0xFFFFFFFF;
	return 0;
}

int Trans_invIP(unsigned l, unsigned r, unsigned long* output){
	unsigned long tmp = ((unsigned long)l)<<32 | (unsigned long)r;
	*output = 0;
	for(int i=0; i<64; i++){
		*output <<= 1;
		if(tmp&(1UL<<IP_1[i])) *output |= 1;
	}
	return 0;
}

#ifdef TEST_MODE
int Trans_Test(unsigned long* m, unsigned* l, unsigned* r){
	if(!*m) *m = 0x0F0F0F0F0F0F0F0F;
	printbinary("m = \t\t", (long)*m, 64);
	printf("Trans_IP\n");
	printf("IP = \n{\n");
	for(int i=0; i<8; i++){
		printf("\t");
		for(int j=0; j<8; j++){
			printf("%2d, ",IP_1[i*8+j]);
		}
		printf("\n");
	}
	Trans_IP(*m, l, r);
	printbinary("l = \t\t", (long)l, 32);
	printbinary("r = \t\t\t\t\t\t", (long)r, 32);
	unsigned long newm;
	Trans_invIP(*l, *r, &newm);
	printbinary("new m = \t", (long)newm, 64);
	if(*m!=newm){
		printf("Fail : Transposed 'm' did not match original 'm'!\n");
		return TEST_FAIL;
	}else{
		printf("Success : Transposed 'm' matched original 'm'!\n");
		return TEST_SUCCESS;
	}
}
#endif /*TEST_MODE*/
