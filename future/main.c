/* gist: code for future cipher in C. The test vectors are given by authors. The oracle has passed */
/* all the test vectors. */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define NO_OF_ROUNDS 10
#define STATE_SIZE 64
#define KEY_SIZE 128

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc(STATE_SIZE);
    uint64_t *key = mem_alloc(KEY_SIZE);

    /* /1* test vector cipher *1/ */
    /* msg[0] = 0x0000000000000000; */
    /* insert(key, 0x0000000000000000, 0x0); */

    /* /1* test vector cipher *1/ */
    /* msg[0] = 0x0000000000000000; */
    /* insert(key, 0x0000000000000000, 0x1111111111111111); */

    /* /1* test vector cipher *1/ */
    /* msg[0] = 0xffffffffffffffff; */
    /* insert(key, 0x0001020304050607, 0x08090a0b0c0d0e0f); */

    /* /1* test vector cipher *1/ */
    /* msg[0] = 0xffffffffffffffff; */
    /* insert(key, 0xffffffffffffffff, 0xffffffffffffffff); */

    /* /1* test vector cipher *1/ */
    /* msg[0] = 0x6162636465666768; */
    /* insert(key, 0x0, 0x0); */

    /* test vector cipher */
    msg[0] = 0x5353414d414e5441;
    insert(key, 0x0519283201091364, 0x5029387763948871);

    oracle(msg, key);

    printf("\ncip: ");
    print(msg, STATE_SIZE);
    }
