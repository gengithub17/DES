#include <stdio.h>
#include <stdlib.h>

extern int Init_SBox(int**** sbox);
extern int Round(unsigned l, unsigned r, long k, unsigned* newl, unsigned* newr, int*** sbox);

#ifdef TEST_MODE
extern int f(unsigned r, long k, unsigned* ret, int*** sbox);
extern int getval_SBox(int** sbox, int inputnum, int* outputnum);
extern int Round_Test(unsigned l, unsigned r, long k, int*** sbox);
#endif /*TEST_MODE*/