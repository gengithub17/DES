extern int Trans_IP(unsigned long m, unsigned* l, unsigned* r);
extern int Trans_invIP(unsigned l, unsigned r, unsigned long* output);
#ifdef TEST_MODE
extern int Trans_Test(unsigned long* m, unsigned* l, unsigned* r);
#endif /*TEST_MODE*/
