/* gist: code for print-48 cipher in C. */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

/* for print-48 the state size is 48, but we save that in 64 bits */
#define STATE_SIZE 64
#define KEY_SIZE 64

#define VERSION 48

#include "my_lib.h"
#include "oracle.h"


int main(){
    uint64_t *msg = mem_alloc(STATE_SIZE);
    uint64_t *key = mem_alloc(KEY_SIZE);

    msg[0] = 0x4C847555C35B;
    key[0] = 0xC28895BA327B;

    uint64_t perm_key = 0x69D2CDB6;
 
    oracle(msg, key, perm_key);

    printf("\ncip: ");
    print(msg, STATE_SIZE);
    }
