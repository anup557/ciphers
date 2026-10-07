/* If the print details is true then only all the state values will print */
char *print_details = "false";
/* char* print_details = "true"; */

/* right shift within 16-bit only */
uint16_t circ_right_shift(uint16_t x, int pos){    
    return ((x>>pos)|(x<<(16-pos)));
    }    


void generate_round_keys(uint64_t *round_key){  
    for(uint8_t round=0; round<NO_OF_ROUNDS; round++){
        uint16_t k[8] = {0};
    
        for(int i=0;i<8;i++){
            uint8_t half = 0;
            /* the lsb 64-bits are in the 1th half */
            if (i<4){
                half = 1;
                }

            /* taking 16-bit keys into k[i]'s */
            k[i] = (round_key[2*round + half] >> 16*i)&0xffff;
            }

        /* making the keys for the next round */
        insert(&round_key[2*(round+1)], 0x0, circ_right_shift(k[1],2)&0xffff);
        shift(&round_key[2*(round+1)], 16, 128, "left");
        round_key[2*(round+1) + 1] |= circ_right_shift(k[0], 12)&0xffff;

        for(int i=7;i>=2;i--){
            shift(&round_key[2*(round+1)], 16, 128, "left");
            round_key[2*(round+1) + 1] |= k[i]&0xffff;
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


void sbox(uint64_t *msg){
    /* sbox table of baksheesh cipher */
    uint8_t sbox_table[16] = {0x1, 0xa, 0x4, 0xc, 0x6, 0xf, 0x3, 0x9, 0x2, 0xd, 0xb, 0x7, 0x5, 0x0, 0x8, 0xe};

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
    if (print_details == "true"){
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
    if (print_details == "true"){
        printf("after p-layer:\t");
        print(msg, 128);
        }
    }


void add_round_key(uint64_t *msg, uint64_t *key){
    /* taking the corresponding 16-bits in u and v */
    uint32_t u = key[0]&0xffffffff;
    uint32_t v = key[1]&0xffffffff;

    uint64_t *u1 = mem_alloc(128);
    uint64_t *v1 = mem_alloc(128);

    /* making u1 and v1 from u and v of the round key */
    for(uint8_t i=0;i<32;i++){
        /* for nibbles [16, 31], half will be 0, and it shifts on left at each time */
        uint8_t half = i/16;

        v1[half] = (v1[half] << 4) | ((v>>(31-i))&1);
        u1[half] = (u1[half] << 4) | ((u>>(31-i))&1);
        }

    shift(u1, 2, STATE_SIZE, "left");
    shift(v1, 1, STATE_SIZE, "left");

    /* xoring u1 and v1 with the msg */
    xr(msg, u1, STATE_SIZE);
    xr(msg, v1, STATE_SIZE);

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add rk:\t");
        print(msg, STATE_SIZE);
        }
    }


void add_rc(uint64_t *msg, uint8_t round_number){
    /* round constants of gift-128 */
    uint8_t rc[48] = {  0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F, 0x1E, 0x3C, 0x39, 0x33, 0x27, 0x0E, 
                        0x1D, 0x3A, 0x35, 0x2B, 0x16, 0x2C, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0B, 0x17, 0x2E, 0x1C, 0x38, 
                        0x31, 0x23, 0x06, 0x0D, 0x1B, 0x36, 0x2D, 0x1A, 0x34, 0x29, 0x12, 0x24, 0x08, 0x11, 0x22, 0x04};

    /* round constant positions */
    uint8_t rcon_pos[6] = {23, 19, 15, 11, 7, 3};

    uint64_t *rcon = mem_alloc(STATE_SIZE);
    for(int i=0;i<6;i++){
        rcon[1] = rcon[1] | ((uint32_t)((rc[round_number]>>i)&1) << rcon_pos[5-i]);
        }

    rcon[0] = rcon[0]|0x8000000000000000;
    xr(msg, rcon, STATE_SIZE);

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add rk:\t");
        print(msg, STATE_SIZE);
        }}


void oracle(uint64_t *msg, uint64_t *key){
    /* allocating mem for round keys */
    uint64_t *round_key = mem_alloc(128*(NO_OF_ROUNDS + 1));

    /* coping the original key into 0-th round key */
    copy(&round_key[0], key, 128);
    generate_round_keys(round_key);

    /* round function */
    for(uint8_t round=0; round < NO_OF_ROUNDS; round++){
        /* for printing purpose */
        if (print_details == "true"){
            printf("\n");
            printf("********************************************************************************\n");
            printf("for round %d:\n", round);
            printf("********************************************************************************\n");
            }

        sbox(msg);
        p_layer(msg);
        add_round_key(msg, &round_key[2*round]);
        add_rc(msg, round);
        }

    if (print_details == "true"){
        printf("********************************************************************************\n");
        }}

