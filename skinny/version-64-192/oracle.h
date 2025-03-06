/* gist: oracle for skinny-64-192 */
/* ------------------------------------------------------------------------------------------ */

/* If the print details is true then only all the state values will print */
/* char *print_details = "false"; */
char* print_details = "true";

/* print the state matrix */
void print_state(uint64_t *msg){
    for (uint8_t row=0; row<4; row++){
        for (uint8_t col=0; col<4; col++){
            printf("%lx\t", (msg[0] >> (60 - 4*col - 16*row))&0xf); 
        }
        printf("\n");
    }
}


void sbox(uint64_t *msg){
    /* sbox table of skinny-64 cipher */
    uint8_t sbox_table[16] = {0xc, 0x6, 0x9, 0x0, 0x1, 0xa, 0x2, 0xb, 0x3, 0x8, 0x5, 0xd, 0x4, 0xe, 0x7, 0xf};

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


void add_rc(uint64_t *msg, uint8_t round_num){
    /* round constants of skinny-64 */
    uint8_t rc[62] = {  0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3e, 0x3d, 0x3b, 0x37, 0x2f, 0x1e, 0x3c, 0x39, 0x33, 0x27, 0x0e,
                        0x1d, 0x3a, 0x35, 0x2b, 0x16, 0x2c, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0b, 0x17, 0x2e, 0x1c, 0x38,
                        0x31, 0x23, 0x06, 0x0d, 0x1b, 0x36, 0x2d, 0x1a, 0x34, 0x29, 0x12, 0x24, 0x08, 0x11, 0x22, 0x04,
                        0x09, 0x13, 0x26, 0x0c, 0x19, 0x32, 0x25, 0x0a, 0x15, 0x2a, 0x14, 0x28, 0x10, 0x20};

    /* forming ci's depending upon the round constants */
    uint64_t c0 = rc[round_num]&0xf;
    uint64_t c1 = (rc[round_num]>>4)&0x3;
    uint64_t c2 = 0x2;

    msg[0] ^= ((c0<<60)|(c1<<44)|(c2<<28));

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add rk:\t");
        print(msg, STATE_SIZE);
    }
}


void sr(uint64_t *msg){
    /* shift row index of skinny state */
    uint8_t sr_index[16] = {0, 1, 2, 3, 7, 4, 5, 6, 10, 11, 8, 9, 13, 14, 15, 12};

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


void mc(uint64_t *msg){
    /* initializing msg to 0 */
    uint64_t cip = 0;

    for (uint8_t col=0; col<4; col++){
        /* storing nibbles into nibble[4]. The nibbles are stored as msg = n0||n1||n2||n3 */
        uint8_t nibble[4] = {0};
        for(uint8_t idx=0; idx<4; idx++){
            nibble[3-idx] = (msg[0]>>(16*idx + 4*col))&0xf;
            }

        uint64_t r[4] = {0};
        r[0] = nibble[0]^nibble[2]^nibble[3];
        r[1] = nibble[0];
        r[2] = nibble[1]^nibble[2];
        r[3] = nibble[0]^nibble[2];

        /* storing resultant nibbles into msg */
        uint64_t tmp = (r[0]<<(48  + 4*col)) | (r[1]<<(32  + 4*col)) | (r[2]<<(16  + 4*col)) | (r[3]<<(0  + 4*col));
        cip |= tmp;
        }

    msg[0] = cip;

    /* for printing purpose */
    if (print_details == "true"){
        printf("after mc:\t");
        print(msg, STATE_SIZE);
    }
}


void add_twk(uint64_t *msg, uint64_t *tk1, uint64_t *tk2, uint64_t *tk3){	
    msg[0] ^= (tk1[0]&0xffffffff00000000)^(tk2[0]&0xffffffff00000000)^(tk3[0]&0xffffffff00000000);

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add-rk:\t");
        print(msg, STATE_SIZE);
    }
}


void generate_twk_key_1(uint64_t *key, uint64_t *twk_key){
    uint8_t pt_index[16] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};

    /* coping original key in the 0-th twk key */
    twk_key[0] = key[0];

    /* generating the tweak keys */
    for (uint8_t i=1; i<NO_OF_ROUNDS; i++){
        uint64_t cip = 0UL;

        /* for each nibble */
        for(int nib_idx=0; nib_idx<16; nib_idx++){
            uint8_t nibble = 0;
            /* taking the nibble according to sr index */
            nibble = (twk_key[i-1]>> (60 - 4*pt_index[nib_idx]))&0xf;

            cip = (cip<<4) | (nibble&0xf);
        }
        twk_key[i] = cip;
    }

    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        printf("tweak keys:\n");
        printf("********************************************************************************\n");
        for (uint8_t i=0; i<NO_OF_ROUNDS; i++){
            printf("for round %d:\t %lx\n", i, twk_key[i]);
        }

        printf("\n");
    }
}


void generate_twk_key_2(uint64_t *key, uint64_t *twk_key){
    uint8_t pt_index[16] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};

    /* coping original key in the 0-th twk key */
    twk_key[0] = key[0];

    /* generating the tweak keys */
    for (uint8_t i=1; i<NO_OF_ROUNDS; i++){
        uint64_t cip = 0UL;

        /* for each nibble */
        for(int nib_idx=0; nib_idx<16; nib_idx++){
            uint8_t nibble = 0;
            /* taking the nibble according to sr index */
            nibble = (twk_key[i-1]>> (60 - 4*pt_index[nib_idx]))&0xf;

            cip = (cip<<4) | (nibble&0xf);
        }

        uint64_t new_cip = cip&0xffffffff;

        for (uint8_t nib_idx=0; nib_idx<8; nib_idx++){
            /* extracting the nibble from the cip where LFSR will apply */
            uint64_t nibble = (cip >> (60 - 4*nib_idx))&0xf;

            /* extracting the bits of the nibble */
            uint8_t x[4] = {0};
            for (uint8_t bit_idx=0; bit_idx<4; bit_idx++){
                x[bit_idx] = (nibble>>bit_idx)&1;
            }

            /* forming the nibble after applying LFSR */
            nibble = ((x[2]<<3)|(x[1]<<2)|(x[0]<<1)|((x[3]^x[2])<<0))&0xf;
            new_cip |= nibble << (60 - 4*nib_idx);
        }

        twk_key[i] = new_cip;
    }

    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        printf("tweak keys:\n");
        printf("********************************************************************************\n");
        for (uint8_t i=0; i<NO_OF_ROUNDS; i++){
            printf("for round %d:\t %lx\n", i, twk_key[i]);
        }

        printf("\n");
    }
}


void generate_twk_key_3(uint64_t *key, uint64_t *twk_key){
    uint8_t pt_index[16] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};

    /* coping original key in the 0-th twk key */
    twk_key[0] = key[0];

    /* generating the tweak keys */
    for (uint8_t i=1; i<NO_OF_ROUNDS; i++){
        uint64_t cip = 0UL;

        /* for each nibble */
        for(int nib_idx=0; nib_idx<16; nib_idx++){
            uint8_t nibble = 0;
            /* taking the nibble according to sr index */
            nibble = (twk_key[i-1]>> (60 - 4*pt_index[nib_idx]))&0xf;

            cip = (cip<<4) | (nibble&0xf);
        }

        uint64_t new_cip = cip&0xffffffff;

        for (uint8_t nib_idx=0; nib_idx<8; nib_idx++){
            /* extracting the nibble from the cip where LFSR will apply */
            uint64_t nibble = (cip >> (60 - 4*nib_idx))&0xf;

            /* extracting the bits of the nibble */
            uint8_t x[4] = {0};
            for (uint8_t bit_idx=0; bit_idx<4; bit_idx++){
                x[bit_idx] = (nibble>>bit_idx)&1;
            }

            /* forming the nibble after applying LFSR */
            nibble = (((x[0]^x[3])<<3)|(x[3]<<2)|(x[2]<<1)|(x[1]<<0))&0xf;
            new_cip |= nibble << (60 - 4*nib_idx);
        }

        twk_key[i] = new_cip;
    }

    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        printf("tweak keys:\n");
        printf("********************************************************************************\n");
        for (uint8_t i=0; i<NO_OF_ROUNDS; i++){
            printf("for round %d:\t %lx\n", i, twk_key[i]);
        }

        printf("\n");
    }
}


void oracle(uint64_t *msg, uint64_t *original_key){
    /* allocating mem for round keys */
    uint64_t *tk1 = mem_alloc(64*NO_OF_ROUNDS);
    uint64_t *tk2 = mem_alloc(64*NO_OF_ROUNDS);
    uint64_t *tk3 = mem_alloc(64*NO_OF_ROUNDS);

    /* coping the original key so that original key doesnot get effected */
    uint64_t *key = mem_alloc(KEY_SIZE);
    copy(key, original_key, KEY_SIZE);

    /* generating the round keys, though its the same for led-64 */
    generate_twk_key_1(&key[0], tk1);
    generate_twk_key_2(&key[1], tk2);
    generate_twk_key_3(&key[2], tk3);

    /* round function */ 
    for (uint8_t i=0; i<NO_OF_ROUNDS; i++){
        /* for printing purpose */
        if (print_details == "true"){
            printf("\n********************************************************************************\n");
            printf("for round %d:\n", i);
            printf("********************************************************************************\n");
        }

        sbox(msg);
        add_rc(msg, i);

        /* the original key is stored in twk-key[0] */
        add_twk(msg, &tk1[i], &tk2[i], &tk3[i]);
        sr(msg);
        mc(msg);
    }

    if (print_details == "true"){
        printf("********************************************************************************\n");
    }
}


