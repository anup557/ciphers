/* gist: code for BAKSHEESH cipher in C. The oracle has passed test vectors given in */ 
/* "https://eprint.iacr.org/2023/750.pdf" (page: 27, table: 14). */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define NO_OF_ROUNDS 35

#include "my_lib.h"
#include "oracle.h"

int main(){
    /* initializing mem for msg and key */
    uint64_t *msg = mem_alloc(128);
    uint64_t *key = mem_alloc(128);

    /* msg and keys based on the last test vector */
    msg[0] = 0xe6517531abf63f3d;    msg[1] = 0x7805e126943a081c;
    key[0] = 0x5920effb52bc61e3;    key[1] = 0x3a98425321e76915;

    /* for printing purpose */
    if (PRINT_DETAILS == "true"){
        printf("\n\n********************************************************************************\n");
        printf("msg: ");
        print(msg, 128);
        printf("key: ");
        print(key, 128);
        }

    oracle(msg, key);

    printf("\ncip: ");
    print(msg, 128);

}

