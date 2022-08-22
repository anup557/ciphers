#include<stdio.h>
#include<stdint.h>
#include "gift_inv_oracle.h"

#define round 28

int main(){
	__uint128_t key;

	long msg, cip = 0xe3272885fa94ba8b;
	long key_0 = 0xbd91731eb6bc2713 , key_1 = 0xa1f9f6ffc75044e7 ;

	key = key_0;
	key = (key<<64)|(key_1&0xffffffffffffffff);
	
	msg = inv_oracle(cip, key, round);
	printf("msg: %lx\n", msg);
	}
