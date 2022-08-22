#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<unistd.h>
#include "present_inv_oracle.h"

#define round_reduced_round 5
#define msg_set_size 4

/* takes 20 sec for 2^21 */


int main(){
	long cip = 0x5579C1387B228445, k_0 = 0, k_1 = 0, msg;
	__uint128_t key;
	
	key = k_0;
	key = key<<64;
	key = key|(k_1&0xffffffffffffffff);

	msg = inv_oracle(cip, key);
	printf("\nmsg: %lx\n\n",msg);
	}

