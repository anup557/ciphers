void ksp(uint16_t *wk, uint16_t *rk, uint64_t *key){
    /* the round constants for piccolo-80 */
    uint32_t rc[50] = { 0x071c, 0x293d, 0x1f1a, 0x253e, 0x1718, 0x213f, 0x2f16, 0x3d38, 0x2714, 0x3939, 
                        0x3f12, 0x353a, 0x3710, 0x313b, 0x4f0e, 0x0d34, 0x470c, 0x0935, 0x5f0a, 0x0536, 
                        0x5708, 0x0137, 0x6f06, 0x1d30, 0x6704, 0x1931, 0x7f02, 0x1532, 0x7700, 0x1133, 
                        0x8f3e, 0x6d2c, 0x873c, 0x692d, 0x9f3a, 0x652e, 0x9738, 0x612f, 0xaf36, 0x7d28, 
                        0xa734, 0x7929, 0xbf32, 0x752a, 0xb730, 0x712b, 0xcf2e, 0x4d24, 0xc72c, 0x4925}; 

    uint16_t k[5];

    k[0] = key[0]&0xffff;
    k[1] = (key[1]>>48)&0xffff;
    k[2] = (key[1]>>32)&0xffff;
    k[3] = (key[1]>>16)&0xffff;
    k[4] = (key[1]>>0)&0xffff;

    /* extracting the whitening keys */
    wk[0] = (k[0]&0xff00) | (k[1]&0x00ff);
    wk[1] = (k[1]&0xff00) | (k[0]&0x00ff);
    wk[2] = (k[4]&0xff00) | (k[3]&0x00ff);
    wk[3] = (k[3]&0xff00) | (k[4]&0x00ff);

    /* generating round keys for the rounds */
    for (uint8_t i=0; i<NO_OF_ROUNDS; i++){
        if (((i%5) == 0) || ((i%5) == 2)){
            rk[2*i]     = k[2]&0xffff;      rk[2*i]     ^= rc[2*i]&0xffff;
            rk[2*i + 1] = k[3]&0xffff;      rk[2*i + 1] ^= rc[2*i + 1]&0xffff;
            }

        if (((i%5) == 1) || ((i%5) == 4)){
            rk[2*i]     = k[0]&0xffff;      rk[2*i]     ^= rc[2*i]&0xffff;
            rk[2*i + 1] = k[1]&0xffff;      rk[2*i + 1] ^= rc[2*i + 1]&0xffff;
            }

        if ((i%5) == 3){
            rk[2*i]     = k[4]&0xffff;      rk[2*i]     ^= rc[2*i]&0xffff;
            rk[2*i + 1] = k[4]&0xffff;      rk[2*i + 1] ^= rc[2*i + 1]&0xffff;
            }}}


void round_perm(uint64_t *msg){
    uint8_t x[8] = {0};
    uint8_t perm_table[8] = {2, 7, 4, 1, 6, 3, 0, 5};

    /* extracting the bytes of msg into x[] */
    for (uint8_t byte=0; byte<8; byte++){
        x[byte] = (msg[0]>>(8*(7-byte)))&0xff;
        }

    /* initializing cip */
    uint64_t cip = 0UL;

    /* putting the extracted bytes into permuted position */
    for (uint8_t byte=0; byte<8; byte++){
        cip = (cip<<8)|(x[perm_table[byte]]&0xff);
        }

    msg[0] = cip;
    }


uint16_t f(uint16_t msg){
    /* storing nibbles into nibble[4]. The nibbles are stored as msg = n0||n1||n2||n3 */
    uint8_t nibble[4] = {0};
    for(uint8_t idx=0; idx<4; idx++){
        nibble[3-idx] = (msg>>(4*idx))&0xf;
        }

    /* ---------------------------------------------------------------------------------------------- */
    /* sbox application */
    /* ---------------------------------------------------------------------------------------------- */
    /* sb table of piccolo */
    uint8_t sb_table[16] = {0xe, 0x4, 0xb, 0x2, 0x3, 0x8, 0x0, 0x9, 0x1, 0xa, 0x7, 0xf, 0x6, 0xc, 0x5, 0xd};

    /* applying sbox into nibbles */
    for(uint8_t idx=0; idx<4; idx++){
        nibble[idx] = sb_table[nibble[idx]];
        }

    printf("\nafter sbox: %x%x%x%x", nibble[0], nibble[1], nibble[2], nibble[3]);

    /* ---------------------------------------------------------------------------------------------- */
    /* MC application */
    /* ---------------------------------------------------------------------------------------------- */
    /* defining multipication table in the GF(2^4) after multiplying by 2 and 3 */
    uint8_t mult_2_field[16] = {0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x3, 0x1, 0x7, 0x5, 0xb, 0x9, 0xf, 0xd};
    uint8_t mult_3_field[16] = {0x0, 0x3, 0x6, 0x5, 0xc, 0xf, 0xa, 0x9, 0xb, 0x8, 0xd, 0xe, 0x7, 0x4, 0x1, 0x2};

    uint8_t r[4] = {0};
    r[0] = mult_2_field[nibble[0]]^mult_3_field[nibble[1]]^nibble[2]^nibble[3];         // 2 * a0 + a3 + a2 + 3 * a1 
    r[1] = nibble[0]^mult_2_field[nibble[1]]^mult_3_field[nibble[2]]^nibble[3];         // 2 * a1 + a0 + a3 + 3 * a2 
    r[2] = nibble[0]^nibble[1]^mult_2_field[nibble[2]]^mult_3_field[nibble[3]];         // 2 * a2 + a1 + a0 + 3 * a3 
    r[3] = mult_3_field[nibble[0]]^nibble[1]^nibble[2]^mult_2_field[nibble[3]];         // 2 * a3 + a2 + a1 + 3 * a0 

    printf("\nafter mc: %x%x%x%x", nibble[0], nibble[1], nibble[2], nibble[3]);
    
    /* ---------------------------------------------------------------------------------------------- */
    /* sbox application */
    /* ---------------------------------------------------------------------------------------------- */
    for(uint8_t idx=0; idx<4; idx++){
        r[idx] = sb_table[r[idx]];
        }

    printf("\nafter f(x): %x%x%x%x\n\n", r[0], r[1], r[2], r[3]);


    /* initializing msg to 0 */
    msg = 0;
    /* storing resultant nibbles into msg */ 
    for (uint8_t idx=0; idx<4; idx++){
        msg = (msg<<4) | (r[idx]&0xf);
        }

    return msg;
    }   


void oracle(uint64_t *msg, uint64_t *original_key){
    /* storing the original key */
    uint64_t *key = mem_alloc(128);
    copy(key, original_key, 128);

    uint16_t *wk = malloc(4*sizeof(uint16_t));
    uint16_t *rk = malloc(2*NO_OF_ROUNDS*sizeof(uint16_t));
    /* applying key schedule to generate four 16-bit whitening keys and 2*NO_OF_ROUNDS 16-bit round keys */
    ksp(wk, rk, key);

    printf("********************************************************************************\n");
    printf("whitening keys: "); 
    for (int i=0; i<4; i++){
        printf("%x ", wk[i]);
    }

    printf("\n********************************************************************************\n");
    printf("\n********************************************************************************\n");
    printf("round keys:\n"); 
    printf("********************************************************************************\n");
    for (int i=0; i<NO_OF_ROUNDS; i++){
        printf("for round %d: %x %x\n", i, rk[2*i], rk[2*i + 1]);
    }
    printf("********************************************************************************\n\n");

    /* storing the msg into four 16-bit words */
    uint16_t *x = malloc(4*sizeof(uint16_t));
    for (uint8_t i=0; i<4; i++){
        x[3-i] = (msg[0]>>(16*i))&0xffff;
        }

    /* applying whitening keys onto the words */
    x[0] ^= wk[0];  x[2] ^= wk[1];

    /* applying the inbetween round functions */
    for (uint8_t i=0; i<(NO_OF_ROUNDS-1); i++){
        x[1] = (x[1]^f(x[0])^rk[2*i]);    x[3] = (x[3]^f(x[2])^rk[2*i + 1]);

        printf("********************************************************************************\n");
        printf("for round %d:", i);
        printf("\n********************************************************************************\n");
        printf("\nafter applying f function,  x0|x1|x2|x3 : %x %x %x %x", x[0], x[1], x[2], x[3]);

        /* making 64-bit tmp msg to apply round permutation */
        uint64_t tmp_msg = 0UL;

        /* making x[0]|x[1]|x[2]|x[3] */
        for (uint8_t i=0; i<4; i++){
            tmp_msg = (tmp_msg<<16)|(x[i]&0xffff);
        }

        round_perm(&tmp_msg);
        printf("\nafter round perm: %lx", tmp_msg);
        printf("\n\n");

        /* extracting 16-bit words from tmp-msg */
        for (uint8_t i=0; i<4; i++){
            x[3-i] = (tmp_msg>>(16*i))&0xffff;
            }
        }

    x[1] = x[1]^f(x[0])^rk[2*NO_OF_ROUNDS - 2];     x[3] = x[3]^f(x[2])^rk[2*NO_OF_ROUNDS - 1];

    printf("\nafter applying f function,  x0|x1|x2|x3 : %x %x %x %x", x[0], x[1], x[2], x[3]);

    /* applying post whitening keys */
    x[0] ^= wk[2];  x[2] ^= wk[3];

    /* storing the resultant four 16-bit words into msg and making x[0]|x[1]|x[2]|x[3] */ 
    msg[0] = 0UL;
    for (uint8_t i=0; i<4; i++){
        msg[0] = (msg[0]<<16)|(x[i]&0xffff);
    }

    printf("\nafter key whitening: %lx", msg[0]);
    printf("\n********************************************************************************\n");
}
