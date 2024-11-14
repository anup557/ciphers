/* gist: this is the cipher code for craft cipher. The test vectors are checked. The test vectors corresponding to the cipher is */ 
/* given in the original paper of craft, named: "CRAFT: Lightweight Tweakable Block Cipher */
/* with Efficient Protection Against DFA Attacks" (url: https://eprint.iacr.org/2019/210.pdf) */
/* --------------------------------------------------------------------------------------------------------------- */

#define rounds 32

/* char* print_details = "true"; */
char* print_details = "false";


/* defining 128-bit state */
struct STATE128{
    uint64_t left,
             right;
};
typedef struct STATE128 state128;

/* defining 256 state */
struct STATE256{
    uint64_t k[4];
};
typedef struct STATE256 state256;


/* changing the orientation of the nibbles within the state */
uint64_t change_endian_nibble(uint64_t msg){
    uint64_t cip = 0UL;

    for (int i=0; i<16; i++){
        cip = (cip<<4) | ((msg >> 4*i)&0xf);
    }
    return cip;
}


uint64_t sbox(uint64_t msg){
    /* sbox table of craft */
    uint8_t sbox_table[16] = {0xc, 0xa, 0xd, 0x3, 0xe, 0xb, 0xf, 0x7, 0x8, 0x9, 0x1, 0x5, 0x0, 0x2, 0x4, 0x6};

    uint64_t cip = 0UL;
    
    /* applying sbox on each nibble */
    for(int8_t i=15;i>=0;i--){
        uint8_t nibble = sbox_table[(msg >> 4*i)&0xf];;
        cip = (cip << 4)|(nibble&0xf);
        }
    
    if (print_details == "true"){
        printf("after sbox:\t %lx\n", change_endian_nibble(cip));
    }
    return cip;
    }


/* mixcol for one col */
uint16_t mc_one_col(uint16_t col){
    /* extracting cells from the col */
    uint8_t cell[4]; 
    for(uint8_t i=0;i<4;i++){
        cell[i] = (col >> 4*i)&0xf;
        }
                 
    /* updating the new cell */
    uint8_t new_cell[4]; 
    new_cell[0] = cell[0]^cell[2]^cell[3];
    new_cell[1] = cell[1]^cell[3];
    new_cell[2] = cell[2];
    new_cell[3] = cell[3];

    /* making 8-bit at a time, as have considered uint8 */
    col = (new_cell[3]<<4)|new_cell[2];
    col = ((col&0xff) << 8)|(new_cell[1]<<4)|new_cell[0];

    return col;
    }

        
uint64_t mc(uint64_t msg){
    uint64_t cip = 0UL; 

    for (uint8_t i=0; i<4; i++){
        /* extracting col */
        uint16_t col =  (((msg>> (48 + 4*i))&0xf) << 12)| (((msg>> (32 + 4*i))&0xf) << 8)|
                        (((msg>> (16 + 4*i))&0xf) << 4) | ((msg>> 4*i)&0xf);
        /* applying mc in the col */
        uint64_t new_col = mc_one_col(col)&0xffff;

        /* placing the col cells in corresp state */
        cip |=   (((new_col>>12)&0xf) << (48 + 4*i))| (((new_col>>8)&0xf) << (32 + 4*i))|
                 (((new_col>>4)&0xf) << (16 + 4*i)) |((new_col&0xf) << 4*i);
        }

    if (print_details == "true"){
        printf("after MC:\t %lx\n", change_endian_nibble(cip));
    }
    return cip;
    }


uint64_t pn(uint64_t msg){
    /* permutation nibble table for craft */
    uint8_t pn_table[16] = {15, 12, 13, 14, 10, 9, 8, 11, 6, 5, 4, 7, 1, 2, 3, 0};

    /* rearranging state depending upon permutation nibble */
    uint64_t cip = 0UL;
    for(uint8_t i=0; i<16; i++){
        cip |= ((msg>> 4*pn_table[i])&0xf) << (4*i);
    }

    if (print_details == "true"){
        printf("after PN:\t %lx\n", change_endian_nibble(cip));
    }
    return cip;
}


uint64_t add_rc(uint64_t msg, uint8_t round_number){
    /* rc table for craft */
    uint8_t rc_table[32] = {0x11, 0x84, 0x42, 0x25, 0x96, 0xc7, 0x63, 0xb1, 0x54, 0xa2, 0xd5, 0xe6, 0xf7, 0x73, 0x31, 0x14,
                            0x82, 0x45, 0x26, 0x97, 0xc3, 0x61, 0xb4, 0x52, 0xa5, 0xd6, 0xe7, 0xf3, 0x71, 0x34, 0x12, 0x85};

    /* idea is: a||b -> (b||a)<<16, as a goes to 4th nibble and b to 5th */
    uint64_t rc_state = ((rc_table[round_number]>>4)&0xf) |
                        ((rc_table[round_number]&0xf) << 4);
    rc_state = (rc_state&0xff) << 16;

    if (print_details == "true"){
        printf("after add rc:\t %lx\n", change_endian_nibble(msg^rc_state));
    }
    return (msg^rc_state);
    }   


/* Q function for the tweakey */
uint64_t Q(uint64_t msg){
    /* nibble table for craft */
    uint8_t Q_table[16] = {12, 10, 15, 5, 14, 8, 9, 2, 11, 3, 7, 4, 6, 0, 1, 13};

    /* rearranging state depending upon Q-table */
    uint64_t cip = 0UL;
    for(uint8_t i=0; i<16; i++){
        cip |= (((msg>> (4*Q_table[i]))&0xf) << (4*i));
    }

    return cip;
}


/* here k0, k1 are the keys. t is the tweakey */
void generate_tweakey(uint64_t k0, uint64_t k1, uint64_t t, state256 *tk){
    tk->k[0] = k0^t;      tk->k[1] = k1^t;
    tk->k[2] = k0^Q(t);   tk->k[3] = k1^Q(t);

    if (print_details == "true"){
        printf("\ntweak keys are:\n\n");

        printf("TK0:\t %lx\n", change_endian_nibble(tk->k[0]));
        printf("TK1:\t %lx\n", change_endian_nibble(tk->k[1]));
        printf("TK2:\t %lx\n", change_endian_nibble(tk->k[2]));
        printf("TK3:\t %lx\n", change_endian_nibble(tk->k[3]));
    }
}


uint64_t add_tweakey(uint64_t msg, uint64_t key){
    if (print_details == "true"){
        printf("after add TK:\t %lx\n", change_endian_nibble(msg^key));
    }
    return (msg^key);
}


uint64_t oracle(uint64_t msg, state256 *tk){
    for (uint8_t i=0; i<rounds; i++){
        if (print_details == "true"){
            printf("\n************************************************************************************\n");
            printf("for round %d: \n", i);
            printf("************************************************************************************\n");
            }

        msg = mc(msg);
        msg = add_rc(msg, i);
        msg = add_tweakey(msg, tk->k[i%4]);

        /* in the last round omit the following functions */
        if (i != 31){
            msg = pn(msg);
            msg = sbox(msg);
        }
    }

    if (print_details == "true"){
        printf("************************************************************************************\n\n");
        }
    return msg;
}
    
