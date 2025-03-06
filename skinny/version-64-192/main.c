/* gist: code for skinny-64-192 cipher in C. */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STATE_SIZE 64
#define KEY_SIZE 192

#define NO_OF_ROUNDS 40

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc(STATE_SIZE);
    uint64_t *key = mem_alloc(KEY_SIZE);

    /* skinny-64-192 test vector given in the paper */
    msg[0] = 0x530c61d35e8663c3;
    key[0] = 0xed00c85b120d6861;    key[1] = 0x8753e24bfd908f60;    key[2] = 0xb2dbb41b422dfcd0;
    
    oracle(msg, key);

    /* for printing purpose */
    printf("\ncip: ");
    print(msg, STATE_SIZE);
    }
