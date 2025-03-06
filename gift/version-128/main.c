/* gist: code for gift-128 cipher in C. The cipher is given in "https://eprint.iacr.org/2017/622.pdf". The oracle has passed */
/* test vectors given in "https://github.com/giftcipher/gift/tree/master/implementations/test%20vectors". */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define NO_OF_ROUNDS 40
#define STATE_SIZE 128

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc(128);
    insert(msg, 0xe39c141fa57dba43, 0xf08a85b6a91f86c1);

    uint64_t *key = mem_alloc(128);
    insert(key, 0xd0f5c59a7700d3e7, 0x99028fa9f90ad837);

    oracle(msg, key);

    printf("\ncip: ");
    print(msg, 128);
    }
