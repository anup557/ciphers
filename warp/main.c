/* gist: code for warp cipher in C. */
/* the states are given in the nibble order (31, 30, ..., 0). */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define NO_OF_ROUNDS 41
#define NO_OF_NIBBLES 32

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint8_t *msg = mem_alloc8(NO_OF_NIBBLES);
    uint8_t *key = mem_alloc8(NO_OF_NIBBLES);

    /* /1* test vector 1 *1/ */
    /* insert8(msg, 0x0123456789abcdef, 0xfedcba9876543210); */ 
    /* insert8(key, 0x0123456789abcdef, 0xfedcba9876543210); */

    /* /1* test vector 2 *1/ */
    /* insert8(msg, 0xffeeddccbbaa9988, 0x7766554433221100); */
    /* insert8(key, 0x0123456789abcdef, 0xfedcba9876543210); */

    /* test vector 3 */
    insert8(msg, 0x1e193d8021dcb798, 0xaae6a5cf09ddc6fa);
    insert8(key, 0x7d3e90b7680c30ee, 0xf745a086f220dca0);

    oracle(msg, key);

    printf("\ncip: ");
    print8(msg, NO_OF_NIBBLES);
    }
