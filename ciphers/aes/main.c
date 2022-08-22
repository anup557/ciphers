#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include "aes_oracle.h"
#include "aes_necessary_functions.h"

__uint128_t _128_from_64_bit(long msg_0, long msg_1){
	__uint128_t msg;
	
	msg = msg_0;
	msg = msg<<64;
	msg = msg|msg_1;
	
	return msg;
	}
	

int main(){
	long 	key_0 = 0x8d2e60365f17c7df, key_1 = 0x1040d7501b4a7b5a,			// key = key_0||key_1 
		msg_0 = 0x59b5088e6dadc3ad, msg_1 = 0x5f27a460872d5929;			// msg = msg_0||msg_1 
	
	__uint128_t msg, round_key[11], key, cip;	
	
	msg = _128_from_64_bit(msg_0, msg_1);
	key = _128_from_64_bit(key_0, key_1);

	cip = oracle(msg, key);
	
	printf("cip: %lx %lx\n",(long)((cip>>64)&0xffffffffffffffff), (long)(cip&0xffffffffffffffff));
	
	}


















