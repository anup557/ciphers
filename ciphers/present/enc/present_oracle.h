int SB_table[16] = {0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2};
int p_layer_table[64] = {0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51, 
			 4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55,
			 8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59,
			12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63};

__uint128_t constant_bit_1 (int bit){					/* this can give constant bits upto 127-bits */
	__uint128_t some_bit_1;
	some_bit_1 = ((long)1 << (bit-64))-1;
	some_bit_1 = (some_bit_1 << 64)|0xffffffffffffffff;
	
	return some_bit_1;
	}							

long round_key[32];

__uint128_t circ_left_shift(__uint128_t x,int pos){			/* circular shifting (in 80-bit) left side by 'pos' position */
	return (((x<<pos)&constant_bit_1(80))|((x>>(80-pos))&constant_bit_1(80)));
	}							


void generate_round_keys(__uint128_t key){
	for(int i=0;i<32;i++){
		round_key[i] = (key>>16)&0xffffffffffffffff;
		key = circ_left_shift(key, 79-18);			
		key = ((__uint128_t)SB_table[(key>>76)&0xf] << 76)|(key&constant_bit_1(76));
		key = key^((i+1) << 15);
		}
	} 


long add_round_key(long state, long round_key){	
	return state^round_key;
	}

	
long SBox(long msg){
	int nibble;
	long cip;
	
	for(int i=15;i>=0;i--){
		nibble = SB_table[(msg >> 4*i)&0xf];
		cip = (cip << 4)|nibble;
		}
	
	return cip;
	}
	
long p_layer(long msg){
	long cip = 0;
	for(int i=0; i < 64; i++)
		cip = cip|(((msg>>i)&1) << p_layer_table[i]);

	return cip;
	}	
	
	
long oracle(long msg, __uint128_t key){
	generate_round_keys(key);

	for(int i=0;i<31;i++){
		msg = add_round_key(msg, round_key[i]);
		msg = SBox(msg);
		msg = p_layer(msg);	
		}	
	msg = add_round_key(msg, round_key[31]);

	return msg;
	}	
	
	
	
long reduced_round_oracle(long msg, __uint128_t key, int round){
	generate_round_keys(key);
		
	for(int i=0;i<round;i++){
		msg = add_round_key(msg, round_key[i]);
		msg = SBox(msg);
		msg = p_layer(msg);	
		}	
	msg = add_round_key(msg, round_key[round]);

	return msg;
	}	
	
	
	
	
	
	
	
	
	
	
	
	
	
