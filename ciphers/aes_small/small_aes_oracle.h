#include<stdio.h>
#define nr 10								/* number of rounds in AES_64 */	

int sb_table[16] = {0x6, 0xb, 0x5, 0x4, 0x2, 0xe, 0x7, 0xa, 0x9, 0xd, 0xf, 0xc, 0x3, 0x1, 0x0, 0x8};	/* the subbyte table */

int SB_one_col(int msg){
	long return_msg = 0;	
	for(int i=0;i<4;i++){
		return_msg = return_msg<<4;
		return_msg = return_msg | sb_table[(msg >> (16 - (4*i + 4)))&0xf];
		}
	return return_msg;
	}
							
long SB(long msg){							/* takes the msg, extract nibble & substitute by subbyte table and return 64 bit modified msg */
	long return_msg = 0;	
	for(int i=0;i<16;i++){
		return_msg = return_msg<<4;
		return_msg = return_msg | sb_table[(msg >> (64 - (4*i + 4)))&0xf];
		}
	return return_msg;
	}
	
long SR(long msg){							/* returns shift row of 64-bit msg */
	char SR_index[16] = {0,5,10,15,4,9,14,3,8,13,2,7,12,1,6,11};	/* storing shift row rule in an array, extracting nibble & checking  where the msg goes by the rule */
	long return_msg = 0;
	
	for(int i=0;i<16;i++)
		return_msg = (return_msg<<4)|(msg>> (60 - (4*SR_index[i]))&0xf);
	return return_msg;
	}
	
int mult_2_field[16] = {0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x3, 0x1, 0x7, 0x5, 0xb, 0x9, 0xf, 0xd};
int mult_3_field[16] = {0x0, 0x3, 0x6, 0x5, 0xc, 0xf, 0xa, 0x9, 0xb, 0x8, 0xd, 0xe, 0x7, 0x4, 0x1, 0x2};

long MC(long msg){					// this MC is using MC table
	long return_msg = 0;	
	unsigned char a[4], r[4];

	for(int column=0; column<4; column++){
		for(int i=0;i<4;i++)
			a[i] = (msg >> (60 - 16*column - 4*i))&0xf;
					
		r[0] = mult_2_field[a[0]]^mult_3_field[a[1]]^a[2]^a[3];			// 2 * a0 + a3 + a2 + 3 * a1 
		r[1] = a[0]^mult_2_field[a[1]]^mult_3_field[a[2]]^a[3];			// 2 * a1 + a0 + a3 + 3 * a2 
		r[2] = a[0]^a[1]^mult_2_field[a[2]]^mult_3_field[a[3]];			// 2 * a2 + a1 + a0 + 3 * a3 
		r[3] = mult_3_field[a[0]]^a[1]^a[2]^mult_2_field[a[3]];			// 2 * a3 + a2 + a1 + 3 * a0 

		for(int i=0;i<4;i++)
			return_msg = return_msg|((long)r[i] << (60 - 16*column - 4*i));
		}
		
	return return_msg;
	}	

int MC_one_col(int msg){
	int return_msg = 0;	
	unsigned char a[4], r[4];
	for(int i=0;i<4;i++)
		a[i] = (msg >> (12 - 4*i))&0xf;
				
	r[0] = mult_2_field[a[0]]^mult_3_field[a[1]]^a[2]^a[3];			// 2 * a0 + a3 + a2 + 3 * a1 
	r[1] = a[0]^mult_2_field[a[1]]^mult_3_field[a[2]]^a[3];			// 2 * a1 + a0 + a3 + 3 * a2 
	r[2] = a[0]^a[1]^mult_2_field[a[2]]^mult_3_field[a[3]];			// 2 * a2 + a1 + a0 + 3 * a3 
	r[3] = mult_3_field[a[0]]^a[1]^a[2]^mult_2_field[a[3]];			// 2 * a3 + a2 + a1 + 3 * a0 
	
	return_msg = ((int)r[0]<<12)|((int)r[1]<<8)|((int)r[2]<<4)|((int)r[3]);
	return return_msg;
	}

/*
long MC(long msg) {							// returns mixcolumn of full state 
	long return_msg = 0;

	for(int column=0; column<4; column++){
		unsigned char a[4], b[4], r[4], h;
		for(int i=0;i<4;i++)
			r[i] = (msg >> (60 - 16*column - 4*i))&0xf;

		for(int i=0; i<4; i++){
			a[i] = r[i];
			h = (r[i] >> 3)&1;		
			b[i] = (r[i] << 1)&0xf; 		
			b[i]^= h*0x3; 					// for irr. poly in Galois field 
			}						
		r[0] = b[0]^a[3]^a[2]^b[1]^a[1]; 			// 2 * a0 + a3 + a2 + 3 * a1 
		r[1] = b[1]^a[0]^a[3]^b[2]^a[2]; 			// 2 * a1 + a0 + a3 + 3 * a2 
		r[2] = b[2]^a[1]^a[0]^b[3]^a[3]; 			// 2 * a2 + a1 + a0 + 3 * a3 
		r[3] = b[3]^a[2]^a[1]^b[0]^a[0]; 			// 2 * a3 + a2 + a1 + 3 * a0 
		
		for(int i=0;i<4;i++)
			return_msg = return_msg|((long)r[i] << (60 - 16*column - 4*i));
		}
	return return_msg;	
	}
*/


long rotword(long msg){						/* takes 4 nibbles, gives rotword of that */
	msg = ((msg<<4)&0xffff)|((msg>>12)&0xf);
	return msg;
	}
	
long subword(long msg){						/* takes 4 nibbles, gives subword of that */
	long new_msg = 0;
	for(int i=0;i<4;i++)
		new_msg = (new_msg<<4)|sb_table[(msg >> (12 - 4*i))&0xf];
		
	return new_msg;
	}

void key_expansion(long key, long* round_key){			/* takes the key and stores round keys in the array depending upon indexes */
	long w[44], rcon[10] = {0x1000, 0x2000, 0x4000, 0x8000, 0x3000, 0x6000, 0xc000, 0xb000, 0x5000, 0xa000};
									/* rcon stores round constants */
	for(int i=0;i<4;i++)
		w[i] = key>> (48 - 16*i);
		
	for(int i=4;i<44;i++){
		long temp = w[i-1];
		if((i%4) == 0)
			temp = subword(rotword(temp))^rcon[(i/4) - 1];
		w[i] = w[i-4]^temp;
		}

	for(int i=0;i<11;i++)
		round_key[i] = ((w[4*i]&0xffff)<<48)|((w[4*i + 1]&0xffff)<<32)|((w[4*i + 2]&0xffff)<<16)|(w[4*i + 3]&0xffff);
	}	
	
	
long oracle(long msg, long key){					/* takes msg, key and returns cipher txt */
	long round_key[11];	
	
	key_expansion(key, &round_key[0]);
	msg = msg^key;
	
	for(int i=0; i<(nr-1); i++)
		msg = MC(SR(SB(msg)))^round_key[i+1];			/* intermediate rounds */
		
	msg = SR(SB(msg))^round_key[10];				/* final round i.e. no mixcolumn */
	return msg;
	}		
		

