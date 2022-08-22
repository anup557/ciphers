
__uint128_t round_key[40];

int rc[48] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F, 0x1E, 0x3C, 0x39, 0x33, 0x27, 0x0E, 0x1D, 0x3A, 0x35, 0x2B, 0x16, 0x2C, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0B, 0x17, 0x2E, 0x1C, 0x38, 0x31, 0x23, 0x06, 0x0D, 0x1B, 0x36, 0x2D, 0x1A, 0x34, 0x29, 0x12, 0x24, 0x08, 0x11, 0x22, 0x04};

int SB_table[16] = {0x1, 0xa, 0x4, 0xc, 0x6, 0xf, 0x3, 0x9, 0x2, 0xd, 0xb, 0x7, 0x5, 0x0, 0x8, 0xe};

uint64_t p_layer_table[128] = {0, 33, 66, 99, 96, 1, 34, 67, 64, 97, 2, 35, 32, 65, 98, 3, 4, 37, 70, 103, 100, 5, 38, 71, 68, 101, 6, 39, 36, 69, 102, 7, 8, 41, 74, 107, 104, 9, 42, 75, 72, 105, 10, 43, 40, 73, 106, 11, 12, 45, 78, 111, 108, 13, 46, 79, 76, 109, 14, 47, 44, 77, 110, 15, 16, 49, 82, 115, 112, 17, 50, 83, 80, 113, 18, 51, 48, 81, 114, 19, 20, 53, 86, 119, 116, 21, 54, 87, 84, 117, 22, 55, 52, 85, 118, 23, 24, 57, 90, 123, 120, 25, 58, 91, 88, 121, 26, 59, 56, 89, 122, 27, 28, 61, 94, 127, 124, 29, 62, 95, 92, 125, 30, 63, 60, 93, 126, 31};

__uint128_t circ_right_shift(__uint128_t x,int pos){		
	return ((((x>>pos)&0xffff)|((x<<(16-pos))&0xffff))&0xffff);
	}							

void generate_round_keys(__uint128_t key){	
	__uint128_t k[8];
	
	round_key[0] = key;
	for(int64_t round=0; round <= no_of_rounds; round++){
		for(int64_t i=0;i<8;i++){
			k[i] = 0;
			k[i] = (round_key[round] >> 16*i)&0xffff;		/* storing 128-bit key in 8 many 16-bits words */
			}

		round_key[round+1] = (circ_right_shift(k[1],2) << 16) | circ_right_shift(k[0],12);		/* updation of keys */
		for(int i=7;i>=2;i--)
			round_key[round+1] = (round_key[round+1]<<16) | k[i];
		}
	}

__uint128_t SBox(__uint128_t msg){
	uint64_t nibble;
	__uint128_t cip;

	for(int64_t i=31;i>=0;i--){
		nibble = SB_table[(msg >> 4*i)&0xf];
		cip = (cip << 4)|nibble;
		}
	
	return cip;
	}
	
__uint128_t p_layer(__uint128_t msg){
	__uint128_t cip = 0;
	for(int64_t i=0;i<128;i++)
		cip = cip|(((msg>>i)&1) << p_layer_table[i]);

	return cip;
	}	
 
__uint128_t add_round_key(__uint128_t msg, __uint128_t key){
	uint64_t u,v;
	__uint128_t u1=0, v1=0;

	v = key&0xffffffff;		/* V = k1|k0 */
	u = (key>>64)&0xffffffff;	/* u = k5|k4 */

	for(int64_t i=0;i<32;i++){
		v1 = (v1 << 4) | ((v>>(31-i))&1);		/* u and v are xor-ed at the positions b_{4i+2} and b_{4i+1} */
		u1 = (u1 << 4) | ((u>>(31-i))&1);
		}
	u1 = u1<<2; 
	v1 = v1<<1; 

	msg = msg^u1^v1;
	return msg;
	}


__uint128_t add_rc(__uint128_t msg, int64_t round_number){
	__uint128_t rcon = 0;
	int64_t rcon_pos[6] = {23, 19, 15, 11, 7, 3};

	for(int64_t i=0;i<6;i++){
		rcon = rcon | (((rc[round_number]>>i)&1) << rcon_pos[5-i]);		/* making rcon similar to msg size so that we can simply xor it */
		}

	__uint128_t rcon_left_constant;
	uint64_t rcon_left_constant_0 = 0x8000000000000000,rcon_left_constant_1 = 0x0;		/* this is to xor 1 at the left of msg */	
        rcon_left_constant = rcon_left_constant_0;
        rcon_left_constant = rcon_left_constant<<64;
        rcon_left_constant = rcon_left_constant|(rcon_left_constant_1&0xffffffffffffffff);

	
	rcon = rcon|rcon_left_constant ;	

	return msg^rcon;
	}


__uint128_t oracle(__uint128_t msg, __uint128_t key){
	generate_round_keys(key);

	for(int64_t round=0; round < no_of_rounds; round++){
		msg = SBox(msg);
		msg = p_layer(msg);	
		msg = add_round_key(msg, round_key[round]);
		msg = add_rc(msg, round);
		}	
	return msg;
	}	
	
