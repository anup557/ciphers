/* ---------------------------------------------------------------------------------------- */
/* gist: oracle for rectangle-80 */
/* ---------------------------------------------------------------------------------------- */

/* If the print details is true then only all the state values will print */
char *print_details = "false";
/* char* print_details = "true"; */

void sbox(uint64_t *msg){
    /* sbox table of rectangle cipher */
    uint8_t sbox_table[16] = {0x6, 0x5, 0xC, 0xA, 0x1, 0xE, 0x7, 0x9, 0xB, 0x0, 0x3, 0xD, 0x8, 0xF, 0x4, 0x2};

    /* extracting the state into 4 rows i.e. msg = row[3]||row[2]||row[1]||row[0] */
    uint16_t *row = (uint16_t*)malloc(4*sizeof(uint16_t));
    for (uint8_t i=0; i<4; i++){
        row[i] = (msg[0]>>(16*(i%4)))&0xffff;
    }

    /* extracting first four cols to apply the sbox */
    uint8_t col[16] = {0};
    for (int8_t j=15; j>=0; j--){
        col[j] = (((row[3]>>j)&1)<<3) | (((row[2]>>j)&1)<<2) | (((row[1]>>j)&1)<<1) | (((row[0]>>j)&1)<<0);
    }

    /* applying sbox on each col nibble */
    for(int8_t j=15;j>=0;j--){
        col[j] = sbox_table[col[j]];;
    }

    /* making all row[]'s to 0 */
    for (uint8_t i=0; i<4; i++){
        row[i] = 0;
    }
    /* making rows from the updated cols */
    for(int8_t j=15;j>=0;j--){
        row[3] |= ((col[j]>>3)&1)<<j;
        row[2] |= ((col[j]>>2)&1)<<j;
        row[1] |= ((col[j]>>1)&1)<<j;
        row[0] |= ((col[j]>>0)&1)<<j;
    }

    /* updating the modified msg from the updated rows */
    msg[0] = 0UL;
    for (int8_t i=3; i>=0; i--){
        msg[0] = (msg[0]<<16) | (row[i]&0xffff);
    }

    /* for printing purpose */
    if (print_details == "true"){
        printf("after sbox:\t");
        print(msg, STATE_SIZE);
    }
}


/* left shift within 16-bit only */
uint16_t circ_left_shift(uint16_t x, int pos){    
    return ((x<<pos)|(x>>(16-pos)));
}    


void shift_row(uint64_t *msg){
    /* extracting the state into 4 rows i.e. msg = row[3]||row[2]||row[1]||row[0] */
    uint16_t *row = (uint16_t*)malloc(4*sizeof(uint16_t));
    for (uint8_t i=0; i<4; i++){
        row[i] = (msg[0]>>(16*(i%4)))&0xffff;
    }

    /* updating the rows */
    row[1] = circ_left_shift(row[1], 1);    
    row[2] = circ_left_shift(row[2], 12);    
    row[3] = circ_left_shift(row[3], 13);    

    /* updating the modified msg from the updated rows */
    msg[0] = 0UL;
    for (int8_t i=3; i>=0; i--){
        msg[0] = (msg[0]<<16) | (row[i]&0xffff);
    }

    /* for printing purpose */
    if (print_details == "true"){
        printf("after sr:\t");
        print(msg, STATE_SIZE);
    }
}


void ksp(uint64_t *round_key, uint64_t *key){
    /* sbox table of rectangle */
    uint8_t sbox_table[16] = {0x6, 0x5, 0xC, 0xA, 0x1, 0xE, 0x7, 0x9, 0xB, 0x0, 0x3, 0xD, 0x8, 0xF, 0x4, 0x2};

    for (uint8_t round=0; round<NO_OF_ROUNDS; round++){
        /* extracting the 64-bit round key = k[3]||k[2]||k[1]||k[0] */
        round_key[round] = key[1];

        /* storing 80-bit key in 5 rows. k = k[4]||k[3]||...||k[0] */
        uint16_t *row = (uint16_t*)malloc(5*sizeof(uint16_t));
        for (uint8_t i=0; i<=4; i++){
            /* ms 64-bit stored in key[1] */
            uint8_t half = 1- i/4;
            row[i] = (key[half]>>(16*(i%4)))&0xffff;
        }

        /* extracting first four cols to apply the sbox */
        uint8_t col[4] = {0};
        for (int8_t j=3; j>=0; j--){
            col[j] = (((row[3]>>j)&1)<<3) | (((row[2]>>j)&1)<<2) | (((row[1]>>j)&1)<<1) | (((row[0]>>j)&1)<<0);
        }

        /* applying sbox to the right 4 columns */
        for (int8_t j=0; j<4; j++){
            col[j] = sbox_table[col[j]];
        }

        for (int8_t i=3; i>=0; i--){
            /* ommiting the first four col vals for updation */
            row[i] &= 0xfff0;

            /* updating the rows depending upon the col vals */
            row[i] |= (((col[3]>>i)&1)<<3) | (((col[2]>>i)&1)<<2) | (((col[1]>>i)&1)<<1) | (((col[0]>>i)&1)<<0);
        }

        /* updation of the rows */
        uint16_t *new_row = (uint16_t*)malloc(5*sizeof(uint16_t));
        new_row[0] = circ_left_shift(row[0], 8)^row[1];
        new_row[1] = row[2];
        new_row[2] = row[3];
        new_row[3] = circ_left_shift(row[3], 12)^row[4];
        new_row[4] = row[0];

        /* --------------------------------------------------------------------------- */
        /* for adding the round constants */
        /* --------------------------------------------------------------------------- */
        uint8_t rc[25] = {  0x01, 0x02, 0x04, 0x09, 0x12, 0x05, 0x0b, 0x16, 0x0c, 0x19, 0x13, 0x07, 0x0f, 0x1f, 0x1e, 0x1c, 
                            0x18, 0x11, 0x03, 0x06, 0x0d, 0x1b, 0x17, 0x0e, 0x1d};

        /* xoring the rc val */
        new_row[0] ^= rc[round]&0x1f;

        /* making the key val to 0 for updating the key */
        insert(key, 0x0, 0x0);
        for (int8_t i=4; i>=0; i--){
            shift(key, 16, 128, "left");
            /* updating the 80-bit key vals */
            key[1] |= new_row[i]&0xffff;
        }

        free(row);
        free(new_row);
    }

    /* extracting the round key from the updated key */
    round_key[NO_OF_ROUNDS] = key[1];

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


void add_round_key(uint64_t *msg, uint64_t *key){	
	xr(msg, key, STATE_SIZE);

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add-rk:\t");
        print(msg, STATE_SIZE);
    }
}


void oracle(uint64_t *msg, uint64_t *original_key){
    /* allocating mem for round keys */
    uint64_t *round_key = mem_alloc(KEY_SIZE*(NO_OF_ROUNDS+1));

    /* coping the original key so that original key doesnot get effected */
    uint64_t *key = mem_alloc(KEY_SIZE);
    copy(key, original_key, KEY_SIZE);

    /* generating the round keys */
    ksp(round_key, key);

    /* round function */
    for(uint8_t round=0; round < NO_OF_ROUNDS; round++){
        /* for printing purpose */
        if (print_details == "true"){
            printf("\n");
            printf("********************************************************************************\n");
            printf("for round %d:\n", round);
            printf("********************************************************************************\n");
        }

        add_round_key(msg, &round_key[round]);
        sbox(msg);
        shift_row(msg);
    }

    /* for printing purpose */
    if (print_details == "true"){
        printf("\n");
        printf("********************************************************************************\n");
        printf("final key whitening:\n");
        printf("********************************************************************************\n");
    }

    add_round_key(msg, &round_key[NO_OF_ROUNDS]);

    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
    }
}
