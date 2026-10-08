/* gist: oracle for midori-64 */
/* ------------------------------------------------------------------------------------------ */

/* If the print details is true then only all the state values will print */
/* char *print_details = "false"; */
char* print_details = "true";

/* print the state matrix */
void print_state(uint64_t *msg){
    for (uint8_t row=0; row<4; row++){
        for (uint8_t col=0; col<4; col++){
            printf("%lx\t", (msg[0] >> (60 - 16*col - 4*row))&0xf); 
        }
        printf("\n");
    }
}


void sbox(uint64_t *msg){
    /* sbox table of midori-64 cipher */
    uint8_t sbox_table[16] = {0xc, 0xa, 0xd, 0x3, 0xe, 0xb, 0xf, 0x7, 0x8, 0x9, 0x1, 0x5, 0x0, 0x2, 0x4, 0x6};

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
    /* shift row index of midrori state */
    uint8_t sr_index[16] = {0, 10, 5, 15, 14, 4, 11, 1, 9, 3, 12, 6, 7, 13, 2, 8};

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
            nibble[3-idx] = (msg[0]>>(4*idx + 16*col))&0xf;
            }

        uint64_t r[4] = {0};
        r[0] = nibble[1]^nibble[2]^nibble[3];
        r[1] = nibble[0]^nibble[2]^nibble[3];
        r[2] = nibble[0]^nibble[1]^nibble[3];
        r[3] = nibble[0]^nibble[1]^nibble[2];

        /* storing resultant nibbles into msg */
        uint64_t tmp = (r[0]<<(12  + 16*col)) | (r[1]<<(8  + 16*col)) | (r[2]<<(4  + 16*col)) | (r[3]<<(0  + 16*col));
        cip |= tmp;
        }

    msg[0] = cip;

    /* for printing purpose */
    if (print_details == "true"){
        printf("after mc:\t");
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


void generate_rnd_key(uint64_t *key, uint64_t *rnd_key){
    /* the given round constants for midori */
    uint64_t alpha[15] = {0x0001010110110011, 0x0111100011000000, 0x1010010000110101, 0x0110001000010011, 0x0001000001001111, 
                         0x1101000101110000, 0x0000001001100110, 0x0000101111001100, 0x1001010010000001, 0x0100000010111000, 
                         0x0111000110010111, 0x0010001010001110, 0x0101000100110000, 0x1111100011001010, 0x1101111110010000};

    /* getting the round keys */
    for (uint8_t i=0; i<15; i++){
        rnd_key[i] = key[i%2]^alpha[i];
    }

    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        printf("round keys:\n");
        printf("********************************************************************************\n");
        for (uint8_t i=0; i<9; i++){
            printf("for round %d:\t %lx\n", i, rnd_key[i]);
        }

        printf("\n");
    }
}


void oracle(uint64_t *msg, uint64_t *original_key){
    /* allocating mem for round keys */
    uint64_t *rnd_key = mem_alloc(64*NO_OF_ROUNDS);

    /* coping the original key so that original key doesnot get effected */
    uint64_t *key = mem_alloc(KEY_SIZE);
    copy(key, original_key, KEY_SIZE);

    /* generating the round keys, though its the same for led-64 */
    generate_rnd_key(key, rnd_key);

    uint64_t *wk = mem_alloc(64);
    /* generating the whitening key */
    wk[0] = key[0]^key[1];

    /* key whitening */
    add_round_key(msg, wk);

    /* round function */ 
    for (uint8_t i=0; i<NO_OF_ROUNDS; i++){
        /* for printing purpose */
        if (print_details == "true"){
            printf("\n********************************************************************************\n");
            printf("for step %d:\n", i);
            printf("********************************************************************************\n");
        }

        sbox(msg);
        sr(msg);
        mc(msg);
        add_round_key(msg, &rnd_key[i]);
    }

    sbox(msg);
    /* post key-whitening */
    add_round_key(msg, wk);

    if (print_details == "true"){
        printf("********************************************************************************\n");
    }
}


