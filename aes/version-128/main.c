/* gist: code for aes-128 cipher in C. The oracle has passed test vectors */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define STATE_SIZE 128
#define NO_OF_ROUNDS 10

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc((NO_OF_ROUNDS+1)*STATE_SIZE);
    uint64_t *key = mem_alloc(STATE_SIZE);

    insert(msg, 0x54776F204F6E6520, 0x4E696E652054776F);
    insert(key, 0x5468617473206D79, 0x204B756E67204675);

    oracle(msg, key);

    /* for printing purpose */
    printf("\ncip:");
    print(msg + 2*NO_OF_ROUNDS, STATE_SIZE);
    printf("\nin state matrix form:\n");
    print_state(msg + 2*NO_OF_ROUNDS);
    }
