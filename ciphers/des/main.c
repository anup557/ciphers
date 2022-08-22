#include<stdio.h>
#include "des.h"
	
int main(){
	long msg = 0x123456ABCD132536, key = 0xAABB09182736CCDD, cip;
		
	cip = des(msg, key);
	printf("cipher text: %lx\n",cip);
	
/*	key_gen(key);
	for (int i=0;i<16;i++)
		printf("keys for round %d: %lx\n", i+1, k[i]);
	
	printf("\n");

*/


	}	
	
