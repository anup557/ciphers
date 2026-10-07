
/* If the print details is true then only all the state values will print */
/* char *print_details = "false"; */
char* print_details = "true";

/* print the state matrix */
void print_state(uint64_t *msg){
    for (uint8_t row=0; row<4; row++){
        for (uint8_t col=0; col<4; col++){
            printf("%lx\t", (msg[0] >> (60 - 4*row - 16*col))&0xf); 
        }
        printf("\n");
    }   
}


void sbox(uint64_t *msg){
    /* sbox table of future cipher */
    uint8_t sbox_table[16] = {0x1, 0x3, 0x0, 0x2, 0x7, 0xe, 0x4, 0xd, 0x9, 0xa, 0xc, 0x6, 0xf, 0x5, 0x8, 0xb};

    uint64_t cip = 0;

    /* applying sbox on each nibble */
    for(int8_t i=15;i>=0;i--){
        uint8_t nibble = sbox_table[(msg[0] >> 4*i)&0xf];;
        cip = (cip << 4)|(nibble&0xf);
    }

    /* storing the modified msg in cip */
    msg[0] = cip;
    
    /* for printing purpose */
    if (print_details == "true"){
        printf("after sbox:\t");
        print(msg, STATE_SIZE);
    }
}   

 
void sr(uint64_t *msg){
    /* shift row index of future state */
    uint8_t sr_index[16] = {0, 13, 10, 7, 4, 1, 14, 11, 8, 5, 2, 15, 12, 9, 6, 3}; 

    uint64_t cip = 0UL;

    /* for each nibble */
    for(int i=0;i<16;i++){
        uint8_t nibble = 0;

        /* taking the nibble according to sr index */
        nibble = (msg[0]>> (60 - 4*sr_index[i]))&0xf;

        cip = (cip<<4) | (nibble&0xf);
    }

    msg[0] = cip;

    /* for printing purpose */
    if (print_details == "true"){
        printf("after sr:\t");
        print(msg, STATE_SIZE);
    }
}   



void add_round_key(uint64_t *msg, uint64_t *key){
    msg[0] ^= key[0];

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add-rk:\t");
        print(msg, STATE_SIZE);
    }
}



void mc(uint64_t *msg){
    /* initializing msg to 0 */
    uint64_t cip = 0;

    for (uint8_t col=0; col<4; col++){
        /* storing nibbles into nibble[]. The nibbles are stored as msg = n0||n1||n2||n3 */
        uint8_t nibble[4] = {0};
        for(uint8_t idx=0; idx<4; idx++){
            nibble[3-idx] = (msg[0]>>(16*col + 4*idx))&0xf;
            }

        /* multipication table in the GF(2^4) after multiplying by 2 and 4 */
        uint8_t mult_2_field[16] = {0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x3, 0x1, 0x7, 0x5, 0xb, 0x9, 0xf, 0xd};
        uint8_t mult_3_field[16] = {0x0, 0x3, 0x6, 0x5, 0xc, 0xf, 0xa, 0x9, 0xb, 0x8, 0xd, 0xe, 0x7, 0x4, 0x1, 0x2};

        uint8_t mult_8_field[16] = {0x0, 0x8, 0x3, 0xb, 0x6, 0xe, 0x5, 0xd, 0xc, 0x4, 0xf, 0x7, 0xa, 0x2, 0x9, 0x1};
        uint8_t mult_9_field[16] = {0x0, 0x9, 0x1, 0x8, 0x2, 0xb, 0x3, 0xa, 0x4, 0xd, 0x5, 0xc, 0x6, 0xf, 0x7, 0xe};

        uint64_t r[4] = {0};

        r[0] = mult_8_field[nibble[0]]^mult_9_field[nibble[1]]^nibble[2]^mult_8_field[nibble[3]];                         // 8*a0 + 9*a1 + 1*a2 + 8*a3
        r[1] = mult_3_field[nibble[0]]^mult_2_field[nibble[1]]^mult_9_field[nibble[2]]^mult_9_field[nibble[3]];           // 3*a0 + 2*a1 + 9*a2 + 9*a3
        r[2] = mult_2_field[nibble[0]]^mult_3_field[nibble[1]]^mult_8_field[nibble[2]]^mult_9_field[nibble[3]];           // 2*a0 + 3*a1 + 8*a2 + 9*a3
        r[3] = mult_9_field[nibble[0]]^mult_9_field[nibble[1]]^mult_8_field[nibble[2]]^nibble[3];                         // 9*a0 + 9*a1 + 8*a2 + 1*a3


        /* storing resultant nibbles into msg */
        uint64_t tmp = (r[0]<<12) | (r[1]<<8) | (r[2]<<4) | (r[3]<<0);
        cip |= tmp<<(16*col);
        }

    msg[0] = cip;

    /* for printing purpose */
    if (print_details == "true"){
        printf("after mc:\t");
        print(msg, STATE_SIZE);
    }
}


void add_rc(uint64_t *msg, uint8_t round_number){
    /* round constants of future */
    uint64_t rc[10] = { 0x1248248148128124, 0x2481481281241248, 0x4812812412482481, 0x8124124824814812, 0x0000000000000000,
                        0x1248248148128124, 0x2481481281241248, 0x4812812412482481, 0x8124124824814812, 0x0000000000000000};

    /* xoring the corresponding round constant with the state value */
    xr(msg, &rc[round_number], STATE_SIZE);

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add rk:\t");
        print(msg, STATE_SIZE);
    }
}


void generate_round_keys(uint64_t *key, uint64_t *round_key){	
	for(uint8_t i=0; i<=NO_OF_ROUNDS; i++){
        if ((i%2) == 0){
            /* updating the least significant 64-bit */
            uint64_t *tmp = mem_alloc(STATE_SIZE);

            /* storing msb 64-bit in tmp to apply circularly shift */
            tmp[0] = key[0];

            /* shifting circularly the most significant 64-bit */
            circ_shift(&tmp[0], (5 * i/2), 64, "left");

            /* storing the updated value into round key */
            round_key[i] = tmp[0];
        }

        else{
            /* updating the least significant 64-bit */
            uint64_t *tmp = mem_alloc(STATE_SIZE);

            /* storing msb 64-bit in tmp to apply circularly shift */
            tmp[0] = key[1];

            /* shifting circularly the most significant 64-bit */
            circ_shift(&tmp[0], (5 * (uint8_t)(i/2)), 64, "left");

            /* storing the updated value into round key */
            round_key[i] = tmp[0];
        }
    }
    
    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        printf("round keys:\n");
        printf("********************************************************************************\n");

        for(uint8_t round=0; round<=NO_OF_ROUNDS; round++){
            printf("for round %d:\t", round);
            print(&round_key[round], 64);
        }
    }
}


void oracle(uint64_t *msg, uint64_t *original_key){
    /* allocating mem for round keys */
    uint64_t *round_key = mem_alloc(STATE_SIZE*(NO_OF_ROUNDS+1));

    /* storing the original key inot key */
    uint64_t *key = mem_alloc(KEY_SIZE);
    copy(key, original_key, KEY_SIZE);

    /* generating the round keys */
    generate_round_keys(key, round_key);

    /* extracting the whitening key */
    uint64_t wk = original_key[0];

    /* xoring the whitening key */ 
    xr(msg, &wk, STATE_SIZE);

    /* round function */
	for(uint8_t round=1; round < NO_OF_ROUNDS; round++){
        /* for printing purpose */
        if (print_details == "true"){
            printf("\n");
            printf("********************************************************************************\n");
            printf("for round %d:\n", round);
            printf("********************************************************************************\n");
            }

		sbox(msg);
		mc(msg);	
        sr(msg);
		add_round_key(msg, &round_key[round]);
        add_rc(msg, round-1);
		}

    /* for the last round */
    sbox(msg);
    sr(msg);
    add_round_key(msg, &round_key[NO_OF_ROUNDS]);

    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
    }
}
	
