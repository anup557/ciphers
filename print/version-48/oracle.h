/* gist: oracle for print-48 cipher in C. The oracle has passed test vectors given in */ 
/* "https://www.iacr.org/archive/ches2010/62250016/62250016.pdf" (page: 15, table: 6). */
/* ------------------------------------------------------------------------------------------ */

/* char* print_details = "false"; */
char* print_details = "true";

/* permutation berfore sbox layer depending upon the user key */
void sbox_perm(uint64_t *msg, uint64_t user_key){
    /* size depennds upon the VERSION */
    for (uint8_t half=0; half<=(VERSION/64); half++){
        uint64_t cip = 0;

        /* apply sbox in each 3 bits */
        for (int8_t i = ((VERSION - 64*half)/3 - 1); i>=0; i--){
            /* deriving sbox key from the user key */
            uint8_t sbox_key = (user_key >> 2*i)&0x3;

            uint8_t nibble = (msg[half] >> 3*i)&0x7;
            /* depending upon the sbox key the 3-bits will permute */ 
            /* if the sbox key is 1 */
            if(sbox_key == 1){
                nibble = (((nibble>>1)&1) << 2) |(((nibble>>2)&1) << 1) | (((nibble>>0)&1) << 0);  
                }

            /* if the sbox key is 2 */
            if(sbox_key == 2){
                nibble = (((nibble>>2)&1) << 2) |(((nibble>>0)&1) << 1) | (((nibble>>1)&1) << 0);  
                }

            /* if the sbox key is 3 */
            if(sbox_key == 3){
                nibble = (((nibble>>0)&1) << 2) |(((nibble>>1)&1) << 1) | (((nibble>>2)&1) << 0);  
                }

            cip = (cip<<3) | (nibble&0x7);
            }

    msg[half] = cip;
    }
   
    /* for printing purpose */
    if (print_details == "true"){
        printf("after sbox-perm:");
        print(msg, STATE_SIZE);
        }
    }


/* applying sbox depending upon the user key */
void sbox(uint64_t *msg){
    /* sbox table for print cipher */
    uint8_t sbox_table[8] = {0x0, 0x1, 0x3, 0x6, 0x7, 0x4, 0x5, 0x2};

    /* size depennds upon the VERSION */
    for (uint8_t half=0; half<=(VERSION/64); half++){
        uint64_t cip = 0;

        /* apply sbox in each 3 bits */
        for (int8_t i = ((VERSION - 64*half)/3 - 1); i>=0; i--){
            uint8_t nibble = (msg[half] >> 3*i)&0x7;
            cip = (cip<<3) | (sbox_table[nibble]&0x7);
            }

        msg[half] = cip;
        }
   
    /* for printing purpose */
    if (print_details == "true"){
        printf("after sbox:\t");
        print(msg, STATE_SIZE);
        }
    }


void p_layer(uint64_t *msg){
    /* for VERSION-48 */
    if (VERSION == 48){
        /* permutation table for print-48 */
        uint8_t perm_table[48] = {  0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 
                                    1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46, 
                                    2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 44, 47};
        uint64_t cip = 0;

        /* applying permutation table in the state */
        for(uint8_t i=0; i<VERSION; i++){
            uint64_t perm_bit = (msg[0] >> i)&1;
            cip |= perm_bit << perm_table[i];
            }
        msg[0] = cip;
        }

    /* permutation layer for VERSION 96 */
    if (VERSION == 96){
        /* permutation table for print-96 */
        uint8_t perm_table[96] = {  0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 
                                    48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 90, 93, 
                                    1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46, 
                                    49, 52, 55, 58, 61, 64, 67, 70, 73, 76, 79, 82, 85, 88, 91, 94, 
                                    2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 44, 47, 
                                    50, 53, 56, 59, 62, 65, 68, 71, 74, 77, 80, 83, 86, 89, 92, 95}; 
        uint64_t cip[2] = {0};

        for(uint8_t i=0; i<VERSION; i++){
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
        }

    /* for printing purpose */
    if (print_details == "true"){
        printf("after p-layer:\t");
        print(msg, STATE_SIZE);
        }
    }

 


void add_rc(uint64_t *msg, uint8_t round){
    /* round constant table for the whole rounds of the cipher */
    /* uint8_t rcon_table[96] = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7E, 0x7D, 0x7B, 0x77, 0x6F, 0x5F, 0x3E, 0x7C, 0x79, 0x73, */ 
    /*                            0x67, 0x4F, 0x1E, 0x3D, 0x7A, 0x75, 0x6B, 0x57, 0x2E, 0x5C, 0x38, 0x70, 0x61, 0x43, 0x06, 0x0D, */ 
    /*                            0x1B, 0x37, 0x6E, 0x5D, 0x3A, 0x74, 0x69, 0x53, 0x26, 0x4C, 0x18, 0x31, 0x62, 0x45, 0x0A, 0x15, */ 
    /*                            0x2B, 0x56, 0x2C, 0x58, 0x30, 0x60, 0x41, 0x02, 0x05, 0x0B, 0x17, 0x2F, 0x5E, 0x3C, 0x78, 0x71, */ 
    /*                            0x63, 0x47, 0x0E, 0x1D, 0x3B, 0x76, 0x6D, 0x5B, 0x36, 0x6C, 0x59, 0x32, 0x64, 0x49, 0x12, 0x25, */ 
    /*                            0x4A, 0x14, 0x29, 0x52, 0x24, 0x48, 0x10, 0x21, 0x42, 0x04, 0x09, 0x13, 0x27, 0x4E, 0x1C, 0x39}; */

    uint8_t rcon_table[48] = {  0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F, 0x1E, 0x3C, 0x39, 0x33, 0x27, 0x0E, 
                                0x1D, 0x3A, 0x35, 0x2B, 0x16, 0x2C, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0B, 0x17, 0x2E, 0x1C, 0x38, 
                                0x31, 0x23, 0x06, 0x0D, 0x1B, 0x36, 0x2D, 0x1A, 0x34, 0x29, 0x12, 0x24, 0x08, 0x11, 0x22, 0x04};  

    msg[VERSION/64] ^= rcon_table[round]&0xff;

    /* for printing purpose */
    if (print_details == "true"){
        printf("after rc:\t");
        print(msg, STATE_SIZE);
        }
    }


/* xoring the same round key in each of the round */
void add_round_key(uint64_t *msg, uint64_t *key){
    for (uint8_t i=0; i<=(VERSION/64); i++){
        msg[i] ^= key[i];
        }

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add-rk:\t");
        print(msg, STATE_SIZE);
        }
    }


void oracle(uint64_t *msg, uint64_t *key, uint64_t perm_key){
    for (uint8_t round = 0; round < 48; round++){
       /* for printing purpose */
        if (print_details == "true"){
            printf("\n");
            printf("********************************************************************************\n");
            printf("for round %d:\n", round);
            printf("********************************************************************************\n");
            }

        add_round_key(msg, key);
        p_layer(msg);
        add_rc(msg, round);
        sbox_perm(msg, perm_key);
        sbox(msg);
        }
   /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        }
    }
