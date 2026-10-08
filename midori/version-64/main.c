/* gist: code for midori-64 cipher in C. The oracle has passed test vectors given in */ 
/* "https://eprint.iacr.org/2015/1142.pdf" (page 29) */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STATE_SIZE 64
#define KEY_SIZE 128

#define NO_OF_ROUNDS 15

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc(STATE_SIZE);
    uint64_t *key = mem_alloc(KEY_SIZE);

    msg[0] = 0x42c20fd3b586879e;
    insert(key, 0x687ded3b3c85b3f3, 0x5b1009863e2a8cbf);
    
    /* msg[0] = 0x0; */
    /* insert(key, 0x0, 0x0); */
    
    oracle(msg, key);

    /* for printing purpose */
    printf("\ncip: ");
    print(msg, STATE_SIZE);
    }
