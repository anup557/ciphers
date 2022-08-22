#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<unistd.h>
#include "present_oracle.h"

#define round_reduced_round 5
#define msg_set_size 4

/* takes 20 sec for 2^21 */


int main(){
	long msg = 0xffffffffffffffff, k_0 = 0xffff, k_1 = 0xffffffffffffffff,
	 cip;
	__uint128_t key;
	
	key = k_0;
	key = key<<64;
	key = key|(k_1&0xffffffffffffffff);
	
	cip = oracle(msg, key);
	printf("\ncip: %lx\n\n",cip);
	}

