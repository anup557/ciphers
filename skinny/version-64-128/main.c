/* gist: code for skinny-64-128 cipher in C. */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STATE_SIZE 64
#define KEY_SIZE 128

#define NO_OF_ROUNDS 36

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc(STATE_SIZE);
    uint64_t *key = mem_alloc(KEY_SIZE);

    /* skinny-64 test vector given in the paper */
    msg[0] = 0xcf16cfe8fd0f98aa;
    insert(key, 0x9eb93640d088da63, 0x76a39d1c8bea71e1);
    
    oracle(msg, key);

    /* for printing purpose */
    printf("\ncip: ");
    print(msg, STATE_SIZE);
    }
