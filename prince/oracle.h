/* gist: oracle for prince */
/* ------------------------------------------------------------------------------------------ */

/* If the print details is true then only all the state values will print */
/* char *print_details = "false"; */
char* print_details = "true";

void sbox(uint64_t *msg){
    /* sbox table of prince cipher */
    uint8_t sbox_table[16] = {0xb, 0xf, 0x3, 0x2, 0xa, 0xc, 0x9, 0x1, 0x6, 0x7, 0x8, 0x0, 0xe, 0x5, 0xd, 0x4};

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


void inv_sbox(uint64_t *msg){
    /* inv sbox table of prince cipher */
    uint8_t sbox_table[16] = {0xb, 0x7, 0x3, 0x2, 0xf, 0xd, 0x8, 0x9, 0xa, 0x6, 0x4, 0x0, 0x5, 0xe, 0xc, 0x1};

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
        printf("after inv-sbox:\t");
        print(msg, STATE_SIZE);
    }
}


void add_rc(uint64_t *msg, uint8_t round_num){
    /* round constants of prince */
    uint64_t rc[12] = {  0x0000000000000000, 0x13198a2e03707344, 0xa4093822299f31d0, 0x082efa98ec4e6c89, 
                        0x452821e638d01377, 0xbe5466cf34e90c6c, 0x7ef84f78fd955cb1, 0x85840851f1ac43aa, 
                        0xc882d32f25323c54, 0x64a51195e0e3610d, 0xd3b5a399ca0c2399, 0xc0ac29b7c97c50dd};

    msg[0] ^= rc[round_num];

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add-rc:\t");
        print(msg, STATE_SIZE);
    }
}


void add_rk(uint64_t *msg, uint64_t *key){
    msg[0] ^= key[0];

    /* for printing purpose */
    if (print_details == "true"){
        printf("after add-rk:\t");
        print(msg, STATE_SIZE);
    }
}


void sr(uint64_t *msg){
    /* shift row index of prince state */
    uint8_t sr_index[16] = {0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11};

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


void inv_sr(uint64_t *msg){
    /* shift row index of prince state */
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
        printf("after inv-sr:\t");
        print(msg, STATE_SIZE);
        }
    }


void mc(uint64_t *msg){                  
    /* storing the 64x64 matrix like each column is a 64 bit long int, store it in an array M */
    uint64_t M[64] = {  0x0888000000000000, 0x4044000000000000, 0x2202000000000000, 0x1110000000000000,
                    0x8880000000000000, 0x0444000000000000, 0x2022000000000000, 0x1101000000000000,
                    0x8808000000000000, 0x4440000000000000, 0x0222000000000000, 0x1011000000000000,
                    0x8088000000000000, 0x4404000000000000, 0x2220000000000000, 0x0111000000000000,

                    0x0000888000000000, 0x0000044400000000, 0x0000202200000000, 0x0000110100000000,
                    0x0000880800000000, 0x0000444000000000, 0x0000022200000000, 0x0000101100000000,
                    0x0000808800000000, 0x0000440400000000, 0x0000222000000000, 0x0000011100000000, 
                    0x0000088800000000, 0x0000404400000000, 0x0000220200000000, 0x0000111000000000,
            
                    0x0000000088800000, 0x0000000004440000, 0x0000000020220000, 0x0000000011010000,
                    0x0000000088080000, 0x0000000044400000, 0x0000000002220000, 0x0000000010110000,
                    0x0000000080880000, 0x0000000044040000, 0x0000000022200000, 0x0000000001110000,
                    0x0000000008880000, 0x0000000040440000, 0x0000000022020000, 0x0000000011100000,
            
                    0x0000000000000888, 0x0000000000004044, 0x0000000000002202, 0x0000000000001110,
                    0x0000000000008880, 0x0000000000000444, 0x0000000000002022, 0x0000000000001101,
                    0x0000000000008808, 0x0000000000004440, 0x0000000000000222, 0x0000000000001011,
                    0x0000000000008088, 0x0000000000004404, 0x0000000000002220, 0x0000000000000111};

    uint64_t msg_,
             cip = 0;

    for(int i=0;i<64;i++){
        msg_ = msg[0]&M[i];    
        uint8_t msg_bit = 0;    

        for(int j=0;j<64;j++){
            /* extracting msg bits & checking whether xor is 0 after AND */
            msg_bit = ((msg_>>j)&1)^msg_bit;    
        }

        cip = (cip<<1)|msg_bit;
        }

    msg[0] = cip;

    /* for printing purpose */
    if (print_details == "true"){
        printf("after mc:\t");
        print(msg, STATE_SIZE);
    }
}
    

void oracle(uint64_t *msg, uint64_t *original_key){
    /* coping the original key so that original key doesnot get effected */
    uint64_t *key = mem_alloc(KEY_SIZE);
    copy(key, original_key, KEY_SIZE);

    /* initial key-whitening */
    add_rk(msg, &key[0]);

    /* round function */ 
    for (uint8_t i=0; i<NO_OF_ROUNDS-1; i++){
        /* for printing purpose */
        if (print_details == "true"){
            printf("\n********************************************************************************\n");
            printf("for round %d:\n", i);
            printf("********************************************************************************\n");
        }

        add_rk(msg, &key[1]);
        add_rc(msg, i);

        /* for the upper half rounds */
        if (i < 5){
            sbox(msg);
            mc(msg);
            sr(msg);
        }

        /* for the middle round */
        else if (i == 5){
            sbox(msg);
            mc(msg);
            inv_sbox(msg);
        }

        /* for the lower half round */
        else{
            inv_sr(msg);
            mc(msg);
            inv_sbox(msg);
        }

    }

    /* post key-whitening */
    add_rc(msg, NO_OF_ROUNDS-1);
    add_rk(msg, &key[1]);

    /* for generating k0" */
    uint64_t k0_1 = (key[0]>>63)&0xffffffffffffffff;
    msg[0] ^= k0_1;

    /* post key-whitening */
    circ_shift(&key[0], 63, 64, "right");
    add_rk(msg, &key[0]);


    if (print_details == "true"){
        printf("********************************************************************************\n");
    }
}


