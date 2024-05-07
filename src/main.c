#include <stdio.h>
#include <stdlib.h>

#include "Constant.h"
#include "Values.h"
#include "Trans.h"
#include "Round.h"
#include "Key.h"

int encrypt(unsigned long message, long key, int round, int*** sbox, unsigned long* cipher
#ifdef DEBUG_WHOLE
	, unsigned* L, unsigned* R, long* K, int* C, int* D
#endif /*DEBUG_WHOLE*/
){
	printf("*****Encrypt*****\n");
	printbinary("plain = ", message, 64);
	unsigned l,r, newl, newr;
	unsigned long rinput, routput;
	Trans_IP(message, &l, &r);
	int c,d, newc,newd;
	long newk;
	Init_Key(key, &c, &d);
#ifdef DEBUG_WHOLE
	L[0] = l; R[0] = r;
	C[0] = c; D[0] = d;
#endif /*DEBUG_WHOLE*/
	for(int i=0; i<round; i++){
		Key_Manage(i, c, d, &newk, &newc, &newd);
		c = newc; d = newd;
		Round(l, r, newk, &newl, &newr, sbox);
		l = newl; r = newr;
#ifdef DEBUG_WHOLE
		L[i+1] = newl; R[i+1] = newr;
		K[i] = newk;
		C[i+1] = newc; D[i+1] = newd;
#endif /*DEBUG_WHOLE*/
	}
	Trans_invIP(r, l, cipher);
	printbinary("cipher = ", *cipher, 64);
	printf("*****End Encrypt*****\n");
	return 0;
}

int decrypt(unsigned long cipher, long key, int round, int*** sbox, unsigned long* message
#ifdef DEBUG_WHOLE
	, unsigned* L, unsigned* R, long* K, int* C, int* D
#endif /*DEBUG_WHOLE*/
){
	printf("*****Decrypt*****\n");
	printbinary("cipher = ", cipher, 64);
	unsigned l,r, newl, newr;
	Trans_IP(cipher, &r, &l);
	int c,d, newc,newd;
	long newk;
	Init_Key(key, &c, &d);
#ifdef DEBUG_WHOLE
	L[round] = l; R[round] = r;
	C[round] = c; D[round] = d;
#endif /*DEBUG_WHOLE*/
	for(int i=round-1; i>=0; i--){
		Key_invManage(i, c, d, &newk, &newc, &newd);
		c = newc; d = newd;
		Round(r, l, newk, &newr, &newl, sbox);
		l = newl; r = newr;
#ifdef DEBUG_WHOLE
		L[i] = newl; R[i] = newr;
		K[i] = newk;
		C[i] = newc; D[i] = newd;
#endif /*DEBUG_WHOLE*/
	}
	Trans_invIP(l, r, message);
	printbinary("message = ", *message, 64);
	printf("*****End Decrypt*****\n");
	return 0;
}

#ifdef DEBUG_WHOLE
int EncDec_Debug(unsigned long m, long k, int round, int*** sbox){
	unsigned long cipher;
	unsigned long decrypted;

	unsigned L[2][round+1];
	unsigned R[2][round+1];
	long K[2][round];
	int C[2][round+1];
	int D[2][round+1];

	encrypt(m, k, round, sbox, &cipher, L[0], R[0], K[0], C[0], D[0]);
	decrypt(cipher, k, round, sbox, &decrypted, L[1], R[1], K[1], C[1], D[1]);
	
	printf("*****Result*****\n");
	printf("Round Value\n");
	printf("\t|              L               ||              R               |");
	printf("\t|              L               ||              R               |\n");
	for(int i=0; i<round+1; i++){
		printf("Round%d",i);
		printbinary_nolinebreak("\t",(long)L[0][i],32);
		printbinary_nolinebreak("",(long)R[0][i],32);
		printbinary_nolinebreak("\t",(long)L[1][i],32);
		printbinary_nolinebreak("",(long)R[1][i],32);
		if(L[0][i]==L[1][i]){
			printf("\tSame");
		}else{
			printf("\tDiff");
		}
		if(R[0][i]==R[1][i]){
			printf("Same\n");
		}else{
			printf("Diff\n");
		}
	}
	printf("\t|              L               ||              R               |");
	printf("\t|              L               ||              R               |\n");

	printf("Key Value\n");
	for(int i=0; i<round; i++){
		printf("Key%d", i);
		printbinary_nolinebreak("\t",K[0][i],48);
		printbinary_nolinebreak("\t",K[1][i],48);
		if(K[0][i]==K[1][i]){
			printf("\tSame\n");
		}else{
			printf("\tDifferent\n");
		}
	}

	printf("*****Result*****\n");
	printbinary("plain\t\t",m,64);
	printbinary("decrypted\t",decrypted,64);
	if(m==decrypted){
		printf("Success : Correct message output!\n");
	}else{
		printf("Fail : The value of the output message is incorrect...\n");
	}
	return 0;
}
#endif /*DEBUG_WHOLE*/

// int : 2147483647
// long : 9223372036854775807
int main(void){
	int*** sbox;
	Init_SBox(&sbox);
	unsigned long m = 0b0001111101100000100000000000000110011101010011101100101010111000;
	long k = 0b000011000110100000000011110000001000010000001000;
	// unsigned l = 0b00101100000101101100100111110111;
	// unsigned r = 0b00101001001111000100100110000001;
	unsigned long cipher;
#ifndef TEST_MODE
	encrypt(m, k, 16, sbox, &cipher);
	decrypt(cipher, k, 16, sbox, &m);
	return 0;
#else /*TEST_MODE*/
	EncDec_Debug(m,k,16,sbox);
	return 0;
#endif /*TEST_MODE*/
}
