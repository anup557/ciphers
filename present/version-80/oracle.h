/* If the print details is true then only all the state values will print */
char *print_details = "false";
/* char* print_details = "true"; */

void sbox(uint64_t *msg){
    /* sbox table of present cipher */
    uint8_t sbox_table[16] = {0xc, 0x5, 0x6, 0xb, 0x9, 0x0, 0xa, 0xd, 0x3, 0xe, 0xf, 0x8, 0x4, 0x7, 0x1, 0x2};

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
        }}


void p_layer(uint64_t *msg){
    /* permutation table of baksheesh same as gift */
    uint8_t perm_table[64] = { 0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51, 
                               4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55,
                               8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59,
                              12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63};

    /* storing the permutation in cip */
    uint64_t cip = 0;

    for(uint8_t i=0; i<64; i++){
        cip |= (((msg[0] >> i)&1) << perm_table[i]);
        }

    msg[0] = cip;

    /* for printing purpose */
    if (print_details == "true"){
        printf("after p-layer:\t");
        print(msg, STATE_SIZE);
        }}


/* circular shifting (in 80-bit) left side by 'pos' position */
void circ_left_shift80(uint64_t *msg, int pos){
    uint16_t cip0 = msg[0];
    uint64_t cip1 = msg[1];

    msg[0] = ((cip0<<pos) | (cip1>>(64-pos)))&0xffff;
    /* computing lsb 64-bit */
    if (pos>16){
        msg[1] = (cip1<<pos) | ((uint64_t)(cip0&0xffff)<<(pos-16)) | (cip1>>((64-pos)+16));
        }
    else{
        msg[1] = ((cip1<<pos) | (cip0>>(16-pos)));
        }
    }


void generate_round_keys(uint64_t *round_key, uint64_t *key){
    /* sbox table of present cipher */
    uint8_t sbox_table[16] = {0xc, 0x5, 0x6, 0xb, 0x9, 0x0, 0xa, 0xd, 0x3, 0xe, 0xf, 0x8, 0x4, 0x7, 0x1, 0x2};

    for(uint8_t round=0; round<=NO_OF_ROUNDS; round++){
        uint64_t *tmp_key = mem_alloc(128);
        copy(tmp_key, key, 128);

        shift(tmp_key, 16, 128, "right");
        /* storing the least significant 64-bit into round key */
        round_key[round] = tmp_key[1];

        circ_left_shift80(key, 79-18);

        /* key_last_4 is the key bits k79k78k77k76 */ 
        uint8_t key_last_4 = (key[0]>>(76-64))&0xf;

        /* applying sbox on the ms 4-bits */
        key[0] = ((uint64_t)sbox_table[key_last_4]<<12) | (key[0]&0xfff);

        /* xoring the round counter with 5 bits from k15 */
        key[1] = key[1]^((round+1)<<15);
        }

    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        printf("round keys:\n");
        printf("********************************************************************************\n");

        for(uint8_t round=0; round<=NO_OF_ROUNDS; round++){
            printf("for round %d:\t", round);
            print(&round_key[round], 64);
            }}}


void add_round_key(uint64_t *msg, uint64_t *key){	
	xr(msg, key, STATE_SIZE);

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add-rk:\t");
        print(msg, STATE_SIZE);
        }}


void oracle(uint64_t *msg, uint64_t *original_key){
    /* allocating mem for round keys */
    uint64_t *round_key = mem_alloc(KEY_SIZE*(NO_OF_ROUNDS+1));

    /* coping the original key so that original key doesnot get effected */
    uint64_t *key = mem_alloc(KEY_SIZE);
    copy(key, original_key, KEY_SIZE);

    /* generating the round keys */
    generate_round_keys(round_key, key);

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
        p_layer(msg);
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
        }}
