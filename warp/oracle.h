/* gist: this is the oracle for the warp cipher. In this cipher the intermediate states of the cipher */ 
/* are stored in a 8-bit array of size 32. */
/* ----------------------------------------------------------------------------------------- */

/* If the print details is true then only all the state values will print */
/* char *print_details = "false"; */
char* print_details = "true";


void shuf_nib(uint8_t *msg){
    /* shuffle nibble for warp */
    uint8_t perm_table[32] = {  31, 6, 29, 14, 1, 12, 21, 8, 27, 2, 3, 0, 25, 4, 23, 10, 
                                15, 22, 13, 30, 17, 28, 5, 24, 11, 18, 19, 16, 9, 20, 7, 26};

    /* initializing new msg to update the nibble values */
    uint8_t *new_msg = mem_alloc8(NO_OF_NIBBLES);

    /* shuffling the nibbles */
    for (uint8_t i=0; i<NO_OF_NIBBLES; i++){
        new_msg[perm_table[i]] = msg[i];
    }

    /* coping updated msg into original msg */
    copy8(msg, new_msg, NO_OF_NIBBLES);

    /* for printing purpose */
    if (print_details == "true"){
        printf("after shuf-nib:\t");
        print8(msg, NO_OF_NIBBLES);
        }
    }


void add_rc(uint8_t *msg, uint8_t round_number){
    /* round constants of gift-128 */
    uint8_t rc[41] = {  0x04, 0x0c, 0x1c, 0x3c, 0x7c, 0xfc, 0xf8, 0xf4, 0xe8, 0xd4, 0xa8, 0x54, 0xac,
                        0x58, 0xb0, 0x64, 0xcc, 0x98, 0x34, 0x6c, 0xdc, 0xb8, 0x74, 0xec, 0xd8, 0xb4,
                        0x68, 0xd0, 0xa4, 0x48, 0x90, 0x24, 0x4c, 0x9c, 0x38, 0x70, 0xe0, 0xc4, 0x88,
                        0x14, 0x2c};
                               

    /* extracting rc0 and rc1 */
    uint8_t rc0 = (rc[round_number]>>4)&0xf;
    uint8_t rc1 = rc[round_number]&0xf;

    /* xoring rc0 and rc1 into corresponding pos */
    msg[1] ^= rc0;
    msg[3] ^= rc1;

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add rc:\t");
        print8(msg, NO_OF_NIBBLES);
    }
}


void f_function(uint8_t *msg, uint8_t *key, uint8_t round){
    /* sbox table of warp cipher */
    uint8_t sbox_table[16] = {0xc, 0xa, 0xd, 0x3, 0xe, 0xb, 0xf, 0x7, 0x8, 0x9, 0x1, 0x5, 0x0, 0x2, 0x4, 0x6};

    /* applying sbox and xoring rc, key onto the left half of the state */
    for (uint8_t i=0; i<16; i++){
        msg[2*i + 1] = sbox_table[msg[2*i]] ^ key[i + 16*((round-1)%2)] ^ msg[2*i + 1];
    }

    /* for printing purpose */
    if (print_details == "true"){
        printf("after f-func:\t");
        print8(msg, NO_OF_NIBBLES);
    }
}


void oracle(uint8_t *msg, uint8_t *key){
    /* round function */
    for(uint8_t round=1; round < NO_OF_ROUNDS; round++){
        /* for printing purpose */
        if (print_details == "true"){
            printf("\n");
            printf("********************************************************************************\n");
            printf("for round %d:\n", round);
            printf("********************************************************************************\n");
            }

        f_function(msg, key, round);

        add_rc(msg, round-1);
        shuf_nib(msg);
    }

    /* last round */
    /* for printing purpose */
    if (print_details == "true"){
        printf("\n");
        printf("********************************************************************************\n");
        printf("for round %d:\n", NO_OF_ROUNDS);
        printf("********************************************************************************\n");
        }

    f_function(msg, key, NO_OF_ROUNDS);
    /* for (uint8_t i=0; i<16; i++){ */
    /*     /1* k_0[i] is initial 16 nibbles *1/ */
    /*     msg[2*i + 1] = sbox_table[msg[2*i]] ^ key[i] ^ msg[2*i + 1]; */
    /* } */
    add_rc(msg, NO_OF_ROUNDS-1);

    if (print_details == "true"){
        printf("********************************************************************************\n");
    }
}

