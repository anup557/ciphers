int inv_SB_table[16] = {0x5, 0xe, 0xf, 0x8, 0xc, 0x1, 0x2, 0xd, 0xb, 0x4, 0x6, 0x3, 0x0, 0x7, 0x9, 0xa};
int SB_table[16] = {0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2};

int inv_p_layer_table[64] = {	0,  4,  8,  12,  16,  20,  24,  28,  32,  36,  40,  44,  48,  52,  56,  60,
				1,  5,  9,  13,  17,  21,  25,  29,  33,  37,  41,  45,  49,  53,  57,  61,  
				2,  6,  10,  14,  18,  22,  26,  30,  34,  38,  42,  46,  50,  54,  58,  62,  
				3,  7,  11,  15,  19,  23,  27,  31,  35,  39,  43,  47,  51,  55,  59,  63};

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

	
long inv_SBox(long msg){
	int nibble;
	long cip;
	
	for(int i=15;i>=0;i--){
		nibble = inv_SB_table[(msg >> 4*i)&0xf];
		cip = (cip << 4)|nibble;
		}
	
	return cip;
	}
	
long inv_p_layer(long msg){
	long cip = 0;
	for(int i=0; i < 64; i++)
		cip = cip|(((msg>>i)&1) << inv_p_layer_table[i]);

	return cip;
	}	
	
	
long inv_oracle(long msg, __uint128_t key){
	generate_round_keys(key);

	msg = add_round_key(msg, round_key[31]);
	for(int i=0;i<31;i++){
		msg = inv_p_layer(msg);	
		msg = inv_SBox(msg);
		msg = add_round_key(msg, round_key[30-i]);
		}	

	return msg;
	}	
	
	
	
long reduced_round_oracle(long msg, __uint128_t key, int round){
	generate_round_keys(key);
	
	msg = add_round_key(msg, round_key[31]);
	for(int i=0;i<round;i++){
		msg = inv_p_layer(msg);	
		msg = inv_SBox(msg);
		msg = add_round_key(msg, round_key[30-i]);
		}	

	return msg;
	}	
	
	
	
	
	
	
	
	
	
	
	
	
	
