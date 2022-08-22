/* for original PRESENT; version = 16, round = 31 */

#include<stdio.h>
#include "present_small_oracle.h"

#define version 4
#define round 10

int main(){
	long msg, cip;
	__uint128_t key, k_0, k_1;
	
	msg = 0;
	k_0 = 0x0;	k_1 = 0x0;
	key = (k_1<<64)|(k_0&0xffffffffffffffff);

	cip = oracle(version, round, msg, key) ;
	printf("cip: %lx\n\n", cip);
	}


