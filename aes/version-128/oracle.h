
char *print_details = "true";
/* char *print_details = "false"; */

/* print the state matrix */
void print_state(uint64_t *msg){
    for (uint8_t row=0; row<4; row++){
        for (uint8_t col=0; col<4; col++){
            printf("%lx\t", (msg[col/2] >> (56 - 8*row - 32*(col%2)))&0xff); 
            }
        printf("\n");
        }}
    

void sbox(uint64_t *msg){							
    /* the subbyte table of aes */
    uint8_t sb_table[16][16] = {
        {0x63, 0x7c,	0x77,	0x7b,	0xf2,	0x6b,	0x6f,	0xc5,	0x30,	0x01,	0x67,	0x2b,	0xfe,	0xd7,	0xab,	0x76},
        {0xca,	0x82,	0xc9,	0x7d,	0xfa,	0x59,	0x47,	0xf0,	0xad,	0xd4,	0xa2,	0xaf,	0x9c,	0xa4,	0x72,	0xc0},
        {0xb7,	0xfd,	0x93,	0x26,	0x36,	0x3f,	0xf7,	0xcc,	0x34,	0xa5,	0xe5,	0xf1,	0x71,	0xd8,	0x31,	0x15},
        {0x04,	0xc7,	0x23,	0xc3,	0x18,	0x96,	0x05,	0x9a,	0x07,	0x12,	0x80,	0xe2,	0xeb,	0x27,	0xb2,	0x75},
        {0x09,	0x83,	0x2c,	0x1a,	0x1b,	0x6e,	0x5a,	0xa0,	0x52,	0x3b,	0xd6,	0xb3,	0x29,	0xe3,	0x2f,	0x84},
        {0x53,	0xd1,	0x00,	0xed,	0x20,	0xfc,	0xb1,	0x5b,	0x6a,	0xcb,	0xbe,	0x39,	0x4a,	0x4c,	0x58,	0xcf},
        {0xd0,	0xef,	0xaa,	0xfb,	0x43,	0x4d,	0x33,	0x85,	0x45,	0xf9,	0x02,	0x7f,	0x50,	0x3c,	0x9f,	0xa8},
        {0x51,	0xa3,	0x40,	0x8f,	0x92,	0x9d,	0x38,	0xf5,	0xbc,	0xb6,	0xda,	0x21,	0x10,	0xff,	0xf3,	0xd2},
        {0xcd,	0x0c,	0x13,	0xec,	0x5f,	0x97,	0x44,	0x17,	0xc4,	0xa7,	0x7e,	0x3d,	0x64,	0x5d,	0x19,	0x73},
        {0x60,	0x81,	0x4f,	0xdc,	0x22,	0x2a,	0x90,	0x88,	0x46,	0xee,	0xb8,	0x14,	0xde,	0x5e,	0x0b,	0xdb},
        {0xe0,	0x32,	0x3a,	0x0a,	0x49,	0x06,	0x24,	0x5c,	0xc2,	0xd3,	0xac,	0x62,	0x91,	0x95,	0xe4,	0x79},
        {0xe7,	0xc8,	0x37,	0x6d,	0x8d,	0xd5,	0x4e,	0xa9,	0x6c,	0x56,	0xf4,	0xea,	0x65,	0x7a,	0xae,	0x08},
        {0xba,	0x78,	0x25,	0x2e,	0x1c,	0xa6,	0xb4,	0xc6,	0xe8,	0xdd,	0x74,	0x1f,	0x4b,	0xbd,	0x8b,	0x8a},
        {0x70,	0x3e,	0xb5,	0x66,	0x48,	0x03,	0xf6,	0x0e,	0x61,	0x35,	0x57,	0xb9,	0x86,	0xc1,	0x1d,	0x9e},
        {0xe1,	0xf8,	0x98,	0x11,	0x69,	0xd9,	0x8e,	0x94,	0x9b,	0x1e,	0x87,	0xe9,	0xce,	0x55,	0x28,	0xdf},
        {0x8c,	0xa1,	0x89,	0x0d,	0xbf,	0xe6,	0x42,	0x68,	0x41,	0x99,	0x2d,	0x0f,	0xb0,	0x54,	0xbb,	0x16}};
							
    /* for the two halves, lsb and msb 64-bit */
    for (uint8_t half=0; half<2; half++){
        uint64_t cip = 0UL;

        for(uint8_t i=0;i<8;i++){
            /* extracting row and col for the sbox table */
            uint8_t row = (msg[half] >> (60 - 8*i))&0xf;
            uint8_t col = (msg[half] >> (56 - 8*i))&0xf;

            /* substituting the state values depending upon sbox table */
            cip = (cip<<8) | sb_table[row][col];
            }

        msg[half] = cip;
        }

    /* for printing purpose */
    if (print_details == "true"){
        printf("after sbox:\t");
        print(msg, 128);
        }
    }
	

void sr(uint64_t *msg){							
    /* shift row index of aes state */
	uint8_t sr_index[16] = {0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11};	
	
    uint64_t cip[2] = {0UL};
    /* for lsb and msb 64-bits */
    for (uint8_t half=0; half<2; half++){
        for(int i=0;i<8;i++){
            uint8_t byte = 0;

            /* taking the byte according to sr index */
            if (sr_index[8*half + i] < 8){
                byte = (msg[0]>> (56 - 8*sr_index[8*half + i]))&0xff;
                }

            else{
                byte = (msg[1]>> (120 - 8*sr_index[8*half + i]))&0xff;
                }
            cip[half] = (cip[half]<<8) | (byte&0xff);
            }}

    msg[0] = cip[0];    msg[1] = cip[1];

    /* for printing purpose */
    if (print_details == "true"){
        printf("after sr:\t");
        print(msg, 128);
        }
    }


void mc(uint64_t *msg) {							
    uint64_t cip[2] = {0UL};

    /* taking each col */
	for(uint8_t column=0; column<4; column++){
		uint8_t a[4], 
                b[4], 
                r[4], 
                h;

        /* taking the byte cell values of that col into r[] */
		for(uint8_t i=0;i<4;i++){
			r[i] = (msg[column/2] >> (56 - 32*(column%2) - 8*i))&0xff;
            }

        /* in the following the ellaboration of the multipication of each column vector with the AES */ 
        /* MC matrix is given */
        /* -------------------------------------------------------------------------------------- */
        /* Here in a column, the vector is represented as (a[0], a[1], a[2], a[3]). Take a[0]. Now */
        /* a[0] consists of a byte and is an element of GF(2^8). Hence a[0] can be represented as */
        /* c7x^7 + ... + c0x^0. */
        /* We have to store values of 2*a[0] and 3*a[0]. */ 
        /* Now, 2*a[0] means in GF(2^8), its x*(c7x^7 + ... + c0x^0), as 2 (00000010 in binary) means x. */
        /* Hence, */
        /*         2*a[0] = x*(c7x^7 + ... + c0x^0) */
        /*                = c7x^8 + c6x^7 + ... + c0x */
        /*                = c7(x^4 + x^3 + x + 1) + (c6x^7 + ... + c0x) */
        /* here, c7 = h and its extracted in the 3rd line below in the for loop after this its multiplied with */
        /* 0x1b = (x^4 + x^3 + x + 1). Here if the msb (i.e. h) is 1, then the multipication */
        /* returns a value, which is then xored in the last line of the for loop, otherwise 0 is xored. */
        
        /* At last, to find the value 3*a[0], it can be done by (2*a[0] + a[0]) as 2 represents the poly */
        /* 1+x, hence it equals with (2*a[0] + a[0]). */
		for(int i=0; i<4; i++){
			a[i] = r[i];
			h = (r[i] >> 7)&1;		
			b[i] = (r[i] << 1)&0xff; 		
			b[i]^= h*0x1b; 					
			}						
		r[0] = b[0]^a[3]^a[2]^b[1]^a[1]; 			
		r[1] = b[1]^a[0]^a[3]^b[2]^a[2]; 			
		r[2] = b[2]^a[1]^a[0]^b[3]^a[3]; 			
		r[3] = b[3]^a[2]^a[1]^b[0]^a[0]; 			
		
        /* making the 64-bits from the modifying cols */
		for(uint8_t i=0;i<4;i++){
            cip[column/2] = (cip[column/2]<<8)|(r[i]&0xff);
            }
		}	

    msg[0] = cip[0];    msg[1] = cip[1];

    /* for printing purpose */
    if (print_details == "true"){
        printf("after mc:\t");
        print(msg, 128);
        }
	}


/* rotation within 4 bytes, i.e. 32-bits */
uint32_t rotword(uint32_t msg){                           
    msg = ((msg<<8)&0xffffffff)|((msg>>24)&0xff);
    return msg;
    }   
    
/* subbyte on 4 bytes, i.e. 32-bits */
uint32_t subword(uint32_t msg){    
    /* the subbyte table of aes */
    uint8_t sb_table[16][16] = {
        {0x63, 0x7c,	0x77,	0x7b,	0xf2,	0x6b,	0x6f,	0xc5,	0x30,	0x01,	0x67,	0x2b,	0xfe,	0xd7,	0xab,	0x76},
        {0xca,	0x82,	0xc9,	0x7d,	0xfa,	0x59,	0x47,	0xf0,	0xad,	0xd4,	0xa2,	0xaf,	0x9c,	0xa4,	0x72,	0xc0},
        {0xb7,	0xfd,	0x93,	0x26,	0x36,	0x3f,	0xf7,	0xcc,	0x34,	0xa5,	0xe5,	0xf1,	0x71,	0xd8,	0x31,	0x15},
        {0x04,	0xc7,	0x23,	0xc3,	0x18,	0x96,	0x05,	0x9a,	0x07,	0x12,	0x80,	0xe2,	0xeb,	0x27,	0xb2,	0x75},
        {0x09,	0x83,	0x2c,	0x1a,	0x1b,	0x6e,	0x5a,	0xa0,	0x52,	0x3b,	0xd6,	0xb3,	0x29,	0xe3,	0x2f,	0x84},
        {0x53,	0xd1,	0x00,	0xed,	0x20,	0xfc,	0xb1,	0x5b,	0x6a,	0xcb,	0xbe,	0x39,	0x4a,	0x4c,	0x58,	0xcf},
        {0xd0,	0xef,	0xaa,	0xfb,	0x43,	0x4d,	0x33,	0x85,	0x45,	0xf9,	0x02,	0x7f,	0x50,	0x3c,	0x9f,	0xa8},
        {0x51,	0xa3,	0x40,	0x8f,	0x92,	0x9d,	0x38,	0xf5,	0xbc,	0xb6,	0xda,	0x21,	0x10,	0xff,	0xf3,	0xd2},
        {0xcd,	0x0c,	0x13,	0xec,	0x5f,	0x97,	0x44,	0x17,	0xc4,	0xa7,	0x7e,	0x3d,	0x64,	0x5d,	0x19,	0x73},
        {0x60,	0x81,	0x4f,	0xdc,	0x22,	0x2a,	0x90,	0x88,	0x46,	0xee,	0xb8,	0x14,	0xde,	0x5e,	0x0b,	0xdb},
        {0xe0,	0x32,	0x3a,	0x0a,	0x49,	0x06,	0x24,	0x5c,	0xc2,	0xd3,	0xac,	0x62,	0x91,	0x95,	0xe4,	0x79},
        {0xe7,	0xc8,	0x37,	0x6d,	0x8d,	0xd5,	0x4e,	0xa9,	0x6c,	0x56,	0xf4,	0xea,	0x65,	0x7a,	0xae,	0x08},
        {0xba,	0x78,	0x25,	0x2e,	0x1c,	0xa6,	0xb4,	0xc6,	0xe8,	0xdd,	0x74,	0x1f,	0x4b,	0xbd,	0x8b,	0x8a},
        {0x70,	0x3e,	0xb5,	0x66,	0x48,	0x03,	0xf6,	0x0e,	0x61,	0x35,	0x57,	0xb9,	0x86,	0xc1,	0x1d,	0x9e},
        {0xe1,	0xf8,	0x98,	0x11,	0x69,	0xd9,	0x8e,	0x94,	0x9b,	0x1e,	0x87,	0xe9,	0xce,	0x55,	0x28,	0xdf},
        {0x8c,	0xa1,	0x89,	0x0d,	0xbf,	0xe6,	0x42,	0x68,	0x41,	0x99,	0x2d,	0x0f,	0xb0,	0x54,	0xbb,	0x16}};
							
    uint32_t new_msg = 0;
    for(uint8_t i=0;i<4;i++){
        new_msg = (new_msg<<8)|sb_table[(msg >> (28 - 8*i))&0xf][(msg >> (24 - 8*i))&0xf];
        }
    
    return new_msg;
    }   


void key_expansion(uint64_t *round_key, uint64_t *key){    
    /* aes round constants */
    uint32_t rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};
    
    uint32_t w[44]; 
    /* making 32-bit words from w[i]'s */
    for(uint8_t i=0;i<4;i++){
        uint8_t half = i/2;
        w[i] = (key[half] >> (96 - 32*(i%2)))&0xffffffff;
        }

    for(uint8_t i=4;i<44;i++){
        uint32_t temp = w[i-1];
        if((i%4) == 0)
            temp = subword(rotword(temp))^rcon[(i/4) - 1]; 
        w[i] = w[i-4]^temp;
        }

    /* storing w[i]'s into round keys */
    for(uint8_t round=0; round<=NO_OF_ROUNDS; round++){
        for(int j=0;j<4;j++){
            uint8_t half = j/2;
            round_key[2*round + half] = (round_key[2*round + half]<<32)|(w[4*round + j]&0xffffffff);
            }}
    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        printf("round keys:\n");
        printf("********************************************************************************\n");

        for(uint8_t round=0; round<=NO_OF_ROUNDS; round++){
            printf("for round %d:\t", round);
            print(&round_key[2*round], 128);
            }}}


void add_round_key(uint64_t *msg, uint64_t *key){
    xr(msg, key, STATE_SIZE);

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add-rk:\t");
        print(msg, 128);
        }
    }


void oracle(uint64_t *msg, uint64_t *key){
    /* allocating mem for round keys */
    uint64_t *round_key = mem_alloc(128*(NO_OF_ROUNDS + 1));

    /* coping the original key into 0-th round key */
    copy(&round_key[0], key, STATE_SIZE);
    key_expansion(round_key, key);

    /* for printing purpose */
    if (print_details == "true"){
        printf("\n");
        printf("********************************************************************************\n");
        printf("initial key whitening:\n");
        printf("********************************************************************************\n");
        }
    /* round function */
    for(uint8_t round=0; round < NO_OF_ROUNDS; round++){
        /* allocating tmp to store msg at the starting of each round */
        uint64_t *tmp = mem_alloc(STATE_SIZE);
        copy(tmp, msg + 2*round, STATE_SIZE);

        /* adding the initial round key */
        if (round == 0){
            add_round_key(tmp, &round_key[0]);
        }


        /* for printing purpose */
        if (print_details == "true"){
            printf("\n");
            printf("********************************************************************************\n");
            printf("for round %d:\n", round);
            printf("********************************************************************************\n");
            }

        sbox(tmp);
        sr(tmp);
        if (round != (NO_OF_ROUNDS -1)){
            mc(tmp);
            }
        add_round_key(tmp, &round_key[2*(round + 1)]);
        copy(msg + 2*(round+1), tmp, STATE_SIZE);
        }

    if (print_details == "true"){
        printf("********************************************************************************\n");
        }
    }

