/* gist: code for speck-32/64 cipher in C. */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

/* the lower things depends upon speck-32/64 */
#define version 32 
#define n 16
#define m 4
#define T 22
#define ALPHA 7
#define BETA 2

#include "my_lib.h"
#include "oracle.h"

/* state size for simon-32/64 is 32 */
#define STATE_SIZE 64
#define KEY_SIZE 64

int main(){
    uint64_t *msg = mem_alloc(STATE_SIZE);
    uint64_t *key = mem_alloc(KEY_SIZE);

    /* storing 32-bit msg and 64-bit key */
    msg[0] = 0x6574694c;
    key[0] = 0x1918111009080100;
    
    
    oracle(msg, key);
    printf("\ncip: ");
    print(msg, STATE_SIZE);
    }
