#include<stdio.h>
#include<stdint.h>
#include "oracle.h"

int main(){
	long msg = 0xc450c7727a9b8a7d , k_0 = 0xbd91731eb6bc2713, k_1 = 0xa1f9f6ffc75044e7;

	long state = 0x0, cip;
	__uint128_t key = 0;
	
	key = k_0;
	key = key<<64;
	key = key|(k_1&0xffffffffffffffff);
	
	cip = oracle(msg, key);

	printf("cip: %lx\n\n", cip);
	}
