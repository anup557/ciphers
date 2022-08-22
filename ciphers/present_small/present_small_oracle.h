#include<stdint.h>

int SB_table[16] = {0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2};

void generate_perm_table(int n, int* perm_table){
	for(int i=0; i < 4*n-1; i++)
		perm_table[i] = (n*i)%(4*n-1);
	perm_table[4*n-1] = 4*n-1;
	}

int inv_p_layer_table[64] = {0,  8,  16,  24,  32,  40,  48,  56,  1,  9,  17,  25,  33,  41,  49,  57,  2,  10,  18,  26,  34,  42,  50,  58,  3,  11,  19,  27,  35,  43,  51,  59,  4,  12,  20,  28,  36,  44,  52,  60,  5,  13,  21,  29,  37,  45,  53,  61,  6,  14,  22,  30,  38,  46,  54,  62,  7,  15,  23,  31,  39,  47,  55,  63};

__uint128_t constant_bit_1 (int bit){					/* this can give constant bits upto 127-bits */
	__uint128_t some_bit_1;
	some_bit_1 = ((long)1 << (bit-64))-1;
	some_bit_1 = (some_bit_1 << 64)|0xffffffffffffffff;
	
	return some_bit_1;
	}							


__uint128_t circ_left_shift(__uint128_t x,int pos){			/* circular shifting (in 80-bit) left side by 'pos' position */
	return (((x<<pos)&constant_bit_1(80))|((x>>(80-pos))&constant_bit_1(80)));
	}							


void generate_round_keys(int version, __uint128_t key, long* round_key){
	__uint128_t constant_1 = 1;
	constant_1 = (constant_1 << 4*version) - 1;
	
	for(int i=0;i<32;i++){
		round_key[i] = (key>>16)&constant_1;

		key = circ_left_shift(key, 79-18);			
		key = ((__uint128_t)SB_table[(key>>76)&0xf] << 76)|(key&constant_bit_1(76));
		key = key^((i+1) << 15);
		}
	} 


long add_round_key(long state, long round_key){	
	return state^round_key;
	}

	
long SBox(int  version, long msg){
	int nibble;
	long cip;
	
	for(int i = version-1;i>=0;i--){
		nibble = SB_table[(msg >> 4*i)&0xf];
		cip = (cip << 4)|nibble;
		}
	
	return cip;
	}
	

long p_layer(int version, long msg){
	int p_layer_table[64];
	long cip = 0;
	
	generate_perm_table(version, &p_layer_table[0]);

	for(int i=0; i < 4*version; i++)
		cip = cip|(((msg>>i)&1) << p_layer_table[i]);
		
	return cip;
	}	


long oracle(int version, int round, long msg, __uint128_t key){
	long round_key[32];
	generate_round_keys(version, key, &round_key[0]);

	for(int i=0;i<round;i++){
		msg = add_round_key(msg, round_key[i]);
		msg = SBox(version, msg);
		msg = p_layer(version, msg);	
		}	
	msg = add_round_key(msg, round_key[round]);

	return msg;
	}	
	
	
	
/*long reduced_round_oracle(long msg, __uint128_t key, int round, int fault_round, int fault_bit_pos){
	generate_round_keys(key);
		
	for(int i=0;i<round;i++){
		msg = add_round_key(msg, round_key[i]);
		msg = SBox(msg);

	if((fault_bit_pos != (-1)) && (i == fault_round))		// will use -1 for not giving the fault
		msg = msg&(~((long)1<<fault_bit_pos));		// giving fault at fault_bit_pos

		msg = p_layer(msg);	
		}	
	msg = add_round_key(msg, round_key[round]);

	return msg;
	}	
*/	
	
	
/*	
long reduced_round_oracle(long msg, __uint128_t key, int round, int fault_round, int fault_bit_pos1, int fault_bit_pos2){
	generate_round_keys(key);
		
	for(int i=0;i<round;i++){
		msg = add_round_key(msg, round_key[i]);
		msg = SBox(msg);

		if((fault_bit_pos1 != (-1)) && (fault_bit_pos2 != (-1)) && (i == fault_round))		// will use -1 for not giving the fault
			msg = msg&(~((long)1<<fault_bit_pos1))&(~((long)1<<fault_bit_pos2));				// giving fault at fault_bit_pos

		msg = p_layer(msg);	
//		msg = right_rotation(msg, 4);
		}	
	msg = add_round_key(msg, round_key[round]);

	return msg;
	}	
*/	
	
	
	
	
	
	
	
	
	
	
	
	
