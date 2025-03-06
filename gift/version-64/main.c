/* gist: code for gift-64 cipher in C. The cipher is given in "https://eprint.iacr.org/2017/622.pdf". The oracle has passed */
/* test vectors given in "https://github.com/giftcipher/gift/tree/master/implementations/test%20vectors". */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define NO_OF_ROUNDS 28
#define STATE_SIZE 64

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc(64);
    msg[0] = 0xc450c7727a9b8a7d;

    uint64_t *key = mem_alloc(128);
    insert(key, 0xbd91731eb6bc2713, 0xa1f9f6ffc75044e7);

    oracle(msg, key);

    printf("\ncip: ");
    print(msg, 64);
    }
