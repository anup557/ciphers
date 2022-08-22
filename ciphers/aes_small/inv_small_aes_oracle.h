#include<stdio.h>
#define nr 10								/* number of rounds in AES_64 */	

int inv_sb_table[16] = {0xe, 0xd, 0x4, 0xc, 0x3, 0x2, 0x0, 0x6, 0xf, 0x8, 0x7, 0x1, 0xb, 0x9, 0x5, 0xa};	/* the subbyte table */
							
long inv_SB(long msg){							/* takes the msg, extract nibble & substitute by subbyte table and return 64 bit modified msg */
	long return_msg = 0;	
	for(int i=0;i<16;i++){
		return_msg = return_msg<<4;
		return_msg = return_msg | inv_sb_table[(msg >> (64 - (4*i + 4)))&0xf];
		}
	return return_msg;
	}
	
long inv_SR(long msg){							/* returns shift row of 64-bit msg */
	char inv_SR_index[16] = {0, 13, 10, 7, 4, 1, 14, 11, 8, 5, 2, 15, 12, 9, 6, 3};	/* storing shift row rule in an array, extracting nibble & checking  where the msg goes by the rule */
	long return_msg = 0;
	
	for(int i=0;i<16;i++)
		return_msg = (return_msg<<4)|(msg>> (60 - (4*inv_SR_index[i]))&0xf);
	return return_msg;
	}


int mult_9_field[16] = {0x0, 0x9, 0x1, 0x8, 0x2, 0xb, 0x3, 0xa, 0x4, 0xd, 0x5, 0xc, 0x6, 0xf, 0x7, 0xe};
int mult_11_field[16] = {0x0, 0xb, 0x5, 0xe, 0xa, 0x1, 0xf, 0x4, 0x7, 0xc, 0x2, 0x9, 0xd, 0x6, 0x8, 0x3};
int mult_13_field[16] = {0x0, 0xd, 0x9, 0x4, 0x1, 0xc, 0x8, 0x5, 0x2, 0xf, 0xb, 0x6, 0x3, 0xe, 0xa, 0x7};
int mult_14_field[16] = {0x0, 0xe, 0xf, 0x1, 0xd, 0x3, 0x2, 0xc, 0x9, 0x7, 0x6, 0x8, 0x4, 0xa, 0xb, 0x5};


long inv_MC(long msg){					// this inv_MC is using MC table
	long return_msg = 0;	
	unsigned char a[4], r[4];

	for(int column=0; column<4; column++){
		for(int i=0;i<4;i++)
			a[i] = (msg >> (60 - 16*column - 4*i))&0xf;
					
		r[0] = mult_14_field[a[0]]^mult_11_field[a[1]]^mult_13_field[a[2]]^mult_9_field[a[3]];	
		r[1] = mult_9_field[a[0]]^mult_14_field[a[1]]^mult_11_field[a[2]]^mult_13_field[a[3]];	
		r[2] = mult_13_field[a[0]]^mult_9_field[a[1]]^mult_14_field[a[2]]^mult_11_field[a[3]];
		r[3] = mult_11_field[a[0]]^mult_13_field[a[1]]^mult_9_field[a[2]]^mult_14_field[a[3]];

		for(int i=0;i<4;i++)
			return_msg = return_msg|((long)r[i] << (60 - 16*column - 4*i));
		}
		
	return return_msg;
	}	

int inv_MC_one_col(int msg){
	int return_msg = 0;	
	unsigned char a[4], r[4];
	for(int i=0;i<4;i++)
		a[i] = (msg >> (12 - 4*i))&0xf;
								
	r[0] = mult_14_field[a[0]]^mult_11_field[a[1]]^mult_13_field[a[2]]^mult_9_field[a[3]];	
	r[1] = mult_9_field[a[0]]^mult_14_field[a[1]]^mult_11_field[a[2]]^mult_13_field[a[3]];	
	r[2] = mult_13_field[a[0]]^mult_9_field[a[1]]^mult_14_field[a[2]]^mult_11_field[a[3]];
	r[3] = mult_11_field[a[0]]^mult_13_field[a[1]]^mult_9_field[a[2]]^mult_14_field[a[3]];
	
	return_msg = ((int)r[0]<<12)|((int)r[1]<<8)|((int)r[2]<<4)|((int)r[3]);
	return return_msg;
	}

	
// till this	


