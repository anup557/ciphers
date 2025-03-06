/* gist: code for rectangle-80 cipher in C. */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define STATE_SIZE 64
/* for rectangle-80 key size is 80, but in that case also we are storing it in 128 bits */
#define KEY_SIZE 128

#define NO_OF_ROUNDS 25

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc(STATE_SIZE);
    uint64_t *key = mem_alloc(KEY_SIZE);

    msg[0] = 0xffffffffffffffff;
    insert(key, 0xffff, 0xffffffffffffffff);
    
    oracle(msg, key);

    /* for printing purpose */
    printf("\ncip: ");
    print(msg, STATE_SIZE);
    }
