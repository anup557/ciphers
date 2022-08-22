#include<stdlib.h>
int perm_Q[16] = {12, 10, 15, 5, 14, 8, 9, 2, 11, 3, 7, 4, 6, 0, 1, 13};

long rc[32] = {0x11, 0x84, 0x42, 0x25, 0x96, 0xc7, 0x63, 0xb1, 0x54, 0xa2, 0xd5, 0xe6, 0xf7, 0x73, 0x31, 0x14,
		0x82, 0x45, 0x26, 0x97, 0xc3, 0x61, 0xb4, 0x52, 0xa5, 0xd6, 0xe7, 0xf3, 0x71, 0x34, 0x12, 0x85};

long PN_table[16] = {15, 12, 13, 14, 10, 9, 8, 11, 6, 5, 4, 7, 1, 2, 3, 0}; 

long SB_table[16] = {0xc, 0xa, 0xd, 0x3, 0xe, 0xb, 0xf, 0x7, 0x8, 0x9, 0x1, 0x5, 0x0, 0x2, 0x4, 0x6};

long* generate_tweakey(long k0, long k1, long tweak){
	long *tk;
	tk = (long*)malloc(4*sizeof(long));
	
	long Q_tweak = 0;
	for(int i=0;i<16;i++)
		Q_tweak = Q_tweak|((long)((tweak >> (60 - 4*perm_Q[i]))&0xf) << (60 - 4*i));
	
	tk[0] = k0^tweak;
	tk[1] = k1^tweak;
	tk[2] = k0^Q_tweak;
	tk[3] = k1^Q_tweak;
	
	return &tk[0];
	}

	
long MC(long msg){
	int I[4][4];
	
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			I[i][j] = msg>>(60 - 4*j - 16*i)&0xf;
	
	for(int j=0;j<4;j++){
		I[0][j] = I[0][j]^I[2][j]^I[3][j];
		I[1][j] = I[1][j]^I[3][j];
		}

	msg = 0;	
	for(int i=0;i<16;i++)
		msg = (msg<<4)|I[i/4][i%4];

	return msg;
	}

long PN(long msg){
	long cip = 0;
	for(int i=0;i<16;i++)
		cip = cip|(((msg >> (60 - 4*PN_table[i]))&0xf) << (60 - 4*i));

	return cip;
	}


long SB(long msg){
	long cip = 0;
	for(int nibble=0; nibble<16; nibble++){
		cip = cip|(SB_table[(msg >> (60 - 4*nibble))&0xf] << (60 - 4*nibble));
		}
		
	return cip;
	}
	
	
long oracle(long msg, long k0, long k1, long tweak){
	long *tk, cip;
	tk = generate_tweakey(k0, k1, tweak);

	cip = msg;
	for(int i=0;i<32;i++){
		cip = MC(cip);
		
		cip = cip^(rc[i]<<(60-20));		/* xor-ing round constants */

		cip = cip^tk[i%4];
		if(i != 31){
			cip = PN(cip);

			cip = SB(cip);
			}
		}
	
	return cip;
	}

	
	
	
