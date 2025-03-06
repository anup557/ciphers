/* gist: code for skinny-64-64 cipher in C. */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STATE_SIZE 64
#define KEY_SIZE 64

#define NO_OF_ROUNDS 32

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc(STATE_SIZE);
    uint64_t *key = mem_alloc(KEY_SIZE);

    /* skinny-64 test vector given in the paper */
    msg[0] = 0x06034f957724d19d;
    key[0] = 0xf5269826fc681238;
    
    oracle(msg, key);

    /* for printing purpose */
    printf("\ncip: ");
    print(msg, STATE_SIZE);
    }
