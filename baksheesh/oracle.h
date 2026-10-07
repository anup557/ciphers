/* gist: oracle for BAKSHEESH cipher in C. The oracle has passed test vectors given in */ 
/* "https://eprint.iacr.org/2023/750.pdf" (page: 27, table: 14). */
/* ------------------------------------------------------------------------------------------ */

/* if the print details is true then only it prints all the intermediate values */
char *PRINT_DETAILS = "true";

void sbox(uint64_t *msg){
    /* sbox table of baksheesh cipher */
    uint8_t sbox_table[16] = {0x3, 0x0, 0x6, 0xD, 0xB, 0x5, 0x8, 0xE, 0xC, 0xF, 0x9, 0x2, 0x4, 0xA, 0x7, 0x1};

    /* applying sbox on the half */
    for (uint8_t half=0; half<2; half++){
        uint64_t cip = 0;

        /* applying sbox on each nibble */
        for(int8_t i=15;i>=0;i--){
            uint8_t nibble = sbox_table[(msg[half] >> 4*i)&0xf];;
            cip = (cip << 4)|(nibble&0xf);
            }

        msg[half] = cip;
        }
    
    /* for printing purpose */
    if (PRINT_DETAILS == "true"){
        printf("after sbox:\t");
        print(msg, 128);
        }
    }

   
void p_layer(uint64_t *msg){
    /* permutation table of baksheesh same as gift */
    uint8_t perm_table[128] = { 0, 33, 66, 99, 96, 1, 34, 67, 64, 97, 2, 35, 32, 65, 98, 3, 
                                4, 37, 70, 103, 100, 5, 38, 71, 68, 101, 6, 39, 36, 69, 102, 7, 
                                8, 41, 74, 107, 104, 9, 42, 75, 72, 105, 10, 43, 40, 73, 106, 11, 
                                12, 45, 78, 111, 108, 13, 46, 79, 76, 109, 14, 47, 44, 77, 110, 15, 
                                16, 49, 82, 115, 112, 17, 50, 83, 80, 113, 18, 51, 48, 81, 114, 19, 
                                20, 53, 86, 119, 116, 21, 54, 87, 84, 117, 22, 55, 52, 85, 118, 23, 
                                24, 57, 90, 123, 120, 25, 58, 91, 88, 121, 26, 59, 56, 89, 122, 27, 
                                28, 61, 94, 127, 124, 29, 62, 95, 92, 125, 30, 63, 60, 93, 126, 31};

    /* to store permutation values of each half */
    uint64_t cip[2] = {0};

    for(uint8_t i=0; i<128; i++){
        uint8_t half = i/64;

        /* for the bit positions, if the permutation bit is within [0, 63], keep the bit in right side of the */
        /* return msg, else keep it in the left */
        if (perm_table[i] < 64){
            cip[1] |= (((msg[1 - half] >> (i%64))&1) << perm_table[i]);
            }

        else{
            cip[0] |= (((msg[1 - half] >> (i%64))&1) << (perm_table[i] - 64));
            }}

    msg[0] = cip[0];    msg[1] = cip[1];

    /* for printing purpose */
    if (PRINT_DETAILS == "true"){
        printf("after p-layer:\t");
        print(msg, 128);
        }
    }


void add_rc(uint64_t *msg, uint8_t round_number){
    /* round constant table of baksheesh */
    uint8_t rcon_table[35] = {  2, 33, 16, 9, 36, 19, 40, 53, 26, 13, 38, 51, 56, 61, 62, 31, 
                                14, 7, 34, 49, 24, 45, 54, 59, 28, 47, 22, 43, 20, 11, 4, 3, 32, 17, 8};

    /* round contant positions of baksheesh */
    uint8_t rcon_pos[6] = {8, 13, 19, 35, 67, 106}; 

    /* initializing rcon for left and right 64 bits */
    uint64_t rcon[2] = {0};

    /* making the round constant depending upon the round number */
    for(int64_t i=0;i<6;i++){
        /* taking the rc bit in the corresp pos to rcon_bit */
        uint64_t rcon_bit = (rcon_table[round_number]>>i)&1;

        /* the round constant pos is within [64, 127] */
        if (rcon_pos[i] < 64){
            rcon[1] |= rcon_bit << rcon_pos[i];
        }
        /* the round constant pos is within [0, 63] */
        else{
            rcon[0] |= rcon_bit << (rcon_pos[i]-64);
        }}

    /* xoring the round constant */
    msg[0] ^= rcon[0];  msg[1] ^= rcon[1];

    /* for printing purpose */
    if (PRINT_DETAILS == "true"){
        printf("after add-rc:\t");
        print(msg, 128);
        }
    }   


void add_round_key(uint64_t *msg, uint64_t *round_key, uint8_t round){
    /* for the 0-th round, only key xor is there in the key addition */
    if (round == 0){
        xr(msg, round_key, 128);

        /* for printing purpose */
        if (PRINT_DETAILS == "true"){
            printf("\nafter add rk: \t");
            print(msg, 128);
            }
        return;
    }

    circ_shift(round_key, 1, 128, "right");
    xr(msg, round_key, 128);

    /* for printing purpose */
    if (PRINT_DETAILS == "true"){
        printf("\nafter add rk: \t");
        print(msg, 128);
        }
    }


void oracle(uint64_t *msg, uint64_t *key){
    /* round functions */
    for (uint8_t round=0; round<NO_OF_ROUNDS; round++){
        /* for printing purpose */
        if (PRINT_DETAILS == "true"){
            printf("\n");
            printf("********************************************************************************\n");
            printf("for round %d:\n", round);
            printf("********************************************************************************\n");
            }

        add_round_key(msg, key, round);
        sbox(msg);
        p_layer(msg);
        add_rc(msg, round);
        }

    /* for printing purpose */
    if (PRINT_DETAILS == "true"){
        printf("\n");
        printf("********************************************************************************\n");
        printf("for round %d:\n", NO_OF_ROUNDS);
        printf("********************************************************************************\n");
        }

    /* key whitening layer in the last round */
    add_round_key(msg, key, NO_OF_ROUNDS);

    /* for printing purpose */
    if (PRINT_DETAILS == "true"){
        printf("********************************************************************************\n");
        }
    }


