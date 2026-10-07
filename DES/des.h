#include "look_up_tables.h"
#define no_of_rounds 16

long k[16] = {0};							/* this is for round keys */

int circ_shift_in_28_bit(int msg, int pos){				/* left circular shifting the msg to 'pos' position */
	return (((msg<<pos)|(msg>>(28-pos)))&0xfffffff);
	}


long PC_1(long key){
	long reduced_key = 0;
		
	for(int i=0;i<56;i++)						/* appling expansion table */
		reduced_key = reduced_key|(((key>>(64-PC_1_table[i]))&1)<<(55-i));

	return reduced_key;
	}


long PC_2(long round_key){
	long expand_round_key = 0;
	
	for(int i=0;i<48;i++)						/* appling expansion table */
		expand_round_key = expand_round_key|(((round_key>>(56-PC_2_table[i]))&1)<<(47-i));		
	
	return expand_round_key;
	}


void key_gen(long key){
	long c,d;
	long reduced_key_56_bit = 0;
	
	reduced_key_56_bit = PC_1(key);
	
	c = reduced_key_56_bit>>28;
	d = reduced_key_56_bit&0xfffffff;

	for(int rounds=0; rounds<16; rounds++){
		long round_key = 0;
		if(((rounds+1) == 1) || ((rounds+1) == 2) || ((rounds+1) == 9) || ((rounds+1) == 16)){
			c = circ_shift_in_28_bit(c,1);
			d = circ_shift_in_28_bit(d,1);
			}
		else{
			c = circ_shift_in_28_bit(c,2);
			d = circ_shift_in_28_bit(d,2);
			}	

		round_key = (c<<28)|d;
		k[rounds] = PC_2(round_key);
		}
	}									


long SB(long right_48_bit){
	char b;
	long right_48_bit_=0;
	
	for(int i=0;i<8; i++){
		b = (right_48_bit>>(42 - 6*i))&0x3f;
		b = SB_table[i][(((b>>5)&1)<<1)|(b&1)][(b>>1)&0xf];
		right_48_bit_ = right_48_bit_|b<<(28 - 4*i);
		}
	return right_48_bit_;
	}


int f(int right, long round_key){						
	long right_48_bit = 0;
	int right_32_bit, right_32_bit_ = 0;
	
	for(int i=0;i<48;i++)						/* appling expansion table */
		right_48_bit = right_48_bit|((((long)right>>(32-expansion_func_table[i]))&1)<<(47-i));

	right_48_bit = right_48_bit^round_key;
	right_32_bit = SB(right_48_bit);	

	for(int i=0;i<32;i++)						/* appling permutation table */
		right_32_bit_ = right_32_bit_|(((right_32_bit>>(32 - perm_table[i]))&1)<<(31-i));
	
	return right_32_bit_;
	}


/*long initial_perm(long msg){						// initial permutation 		
	long msg1 = 0;
	for(int i=0;i<64;i++)
		msg1 = msg1|(((msg>>(initial_perm_table[i]-1))&1)<<i);

	return msg1;
	}*/

long initial_perm(long msg){						// initial permutation 		//checked
	long msg1 = 0;
	for(int i=0;i<64;i++)
		msg1 = msg1|(((msg>>(64-initial_perm_table[i]))&1)<<(63-i));

	return msg1;
	}
	

long inv_initial_perm(long msg){					/* inv_initial permutation */
	long cip = 0;
	for(int i=0;i<64;i++)
		cip = cip|(((msg>>(64 - inv_initial_perm_table[i]))&1)<<(63-i));
	
	return cip;
	}


long des(long msg, long key){
	int left, right, right_prev;

	key_gen(key);
	msg = initial_perm(msg);

	right = msg&0xffffffff;					/* taking 32-bits to left and another 32-bits to right */
	left = (msg>>32)&0xffffffff;
	
	for(int round=0; round < no_of_rounds; round++){				/* appling rounds in des */	
		right_prev = right;
		right = left^f(right, k[round]);
		left = right_prev;
		}

	msg = ((long)right<<32)|(left&0xffffffff);
	msg = inv_initial_perm(msg);
	return msg;
	}
	
	
	

