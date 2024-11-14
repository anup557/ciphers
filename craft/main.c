/* gist: this is the cipher code for craft cipher. The test vectors are checked. The test vectors corresponding to the cipher is */
/* given in the original paper of craft, named: "CRAFT: Lightweight Tweakable Block Cipher */
/* with Efficient Protection Against DFA Attacks" (url: https://eprint.iacr.org/2019/210.pdf) */
/* --------------------------------------------------------------------------------------------------------------- */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include "oracle.h"

int main(){
    state256 *tk = (state256*)malloc(sizeof(state256));

    /* test vector corresponding to craft paper */
    uint64_t k0 = change_endian_nibble(0x27A6781A43F364BC),
             k1 = change_endian_nibble(0x916708D5FBB5AEFE),
             t = change_endian_nibble(0x54CD94FFD0670A58);
    
    uint64_t msg = change_endian_nibble(0x5734F006D8D88A3E);

    generate_tweakey(k0, k1, t, tk);
    msg = oracle(msg, tk);

    printf("cip: %lx\n", change_endian_nibble(msg));
}

