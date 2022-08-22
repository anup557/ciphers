#include<stdio.h>
#include<inttypes.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<unistd.h>

#define no_of_rounds 40

#include "oracle.h"

int main(){
	/* uint64_t msg_0 = 0xe39c141fa57dba43, msg_1 = 0xf08a85b6a91f86c1; */
	/* uint64_t k_0 = 0xd0f5c59a7700d3e7, k_1 =  0x99028fa9f90ad837; */

	uint64_t msg_0 = 0x0, msg_1 = 0x0;
	uint64_t k_0 = 0x0, k_1 =  0x0;
	__uint128_t msg, key, cip;
	
	key = k_0;
	key = key<<64;
	key = key|(k_1&0xffffffffffffffff);
	
	msg = msg_0;
	msg = msg<<64;
	msg = msg|(msg_1&0xffffffffffffffff);

	cip = oracle(msg, key);
	printf("\ncip: %" PRIx64 "%" PRIx64, (uint64_t)(cip>>64)&0xffffffffffffffff, (uint64_t)(cip&0xffffffffffffffff));
	printf("\n");
	}

