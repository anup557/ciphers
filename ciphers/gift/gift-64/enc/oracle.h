#define  no_of_rounds 28

__uint128_t round_key[40];

int rc[28] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F, 0x1E, 0x3C, 0x39, 0x33, 0x27, 0x0E, 0x1D, 0x3A, 0x35, 0x2B, 0x16, 0x2C, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0B};

int SB_table[16] = {0x1, 0xa, 0x4, 0xc, 0x6, 0xf, 0x3, 0x9, 0x2, 0xd, 0xb, 0x7, 0x5, 0x0, 0x8, 0xe};
int p_layer_table[64] = {   0, 17, 34, 51, 48, 1, 18, 35, 32, 49, 2, 19, 16, 33, 50, 3,														  4, 21, 38, 55, 52, 5, 22, 39, 36, 53, 6, 23, 20, 37, 54, 7,														8, 25, 42, 59, 56, 9, 26, 43, 40, 57, 10, 27, 24, 41, 58, 11,													     12, 29, 46, 63, 60, 13, 30, 47, 44, 61, 14, 31, 28, 45, 62, 15};

int inv_p_layer_table[64] = {	0, 5, 10, 15, 16, 21, 26, 31, 32, 37, 42, 47, 48, 53, 58, 63, 													     12, 1, 6, 11, 28, 17, 22, 27, 44, 33, 38, 43, 60, 49, 54, 59, 													    8, 13, 2, 7, 24, 29, 18, 23, 40, 45, 34, 39, 56, 61, 50, 55, 													  4, 9, 14, 3, 20, 25, 30, 19, 36, 41, 46, 35, 52, 57, 62, 51};

__uint128_t circ_right_shift(__uint128_t x,int pos){		
	return ((((x>>pos)&0xffff)|((x<<(16-pos))&0xffff))&0xffff);
	}							

void generate_round_keys(__uint128_t key){	
	__uint128_t k[8];
	
	round_key[0] = key;
	for(int round=0; round<=28; round++){
		for(int i=0;i<8;i++){
			k[i] = 0;
			k[i] = (round_key[round] >> 16*i)&0xffff;
			}

		round_key[round+1] = (circ_right_shift(k[1],2) << 16) | circ_right_shift(k[0],12);
		for(int i=7;i>=2;i--)
			round_key[round+1] = (round_key[round+1]<<16) | k[i];
		}
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
	for(int i=0;i<64;i++)
		cip = (cip << 1)|((msg >> inv_p_layer_table[63-i])&1);
		
	return cip;
	}	
 
long add_round_key(long msg, __uint128_t key){
	int u,v;
	long u1=0, v1=0;

	v = key&0xffff;
	u = (key>>16)&0xffff;

	for(int i=0;i<16;i++){
		v1 = (v1 << 4) | ((v>>(15-i))&1);
		u1 = (u1 << 4) | ((u>>(15-i))&1);
		}
	u1 = u1<<1; 

	msg = msg^u1^v1;
	return msg;
	}


long add_rc(long msg, int round_number){
	long rcon = 0;
	int rcon_pos[6] = {23, 19, 15, 11, 7, 3};

	for(int i=0;i<6;i++){
		rcon = rcon | (((rc[round_number]>>i)&1) << rcon_pos[5-i]);
		}
	rcon = rcon|0x8000000000000000;	

	return msg^rcon;
	}


long oracle(long msg, __uint128_t key){
	generate_round_keys(key);
		
	for(int round=0; round < no_of_rounds; round++){
		msg = SBox(msg);
		msg = p_layer(msg);	
		msg = add_round_key(msg, round_key[round]);
		msg = add_rc(msg, round);
		}	
	
	return msg;
	}	
	
