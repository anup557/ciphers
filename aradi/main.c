/* gist: code for aradi cipher in C. The oracle has passed test vectors given in */
/* "ARADI and LLAMA: Low-Latency Cryptography for Memory Encryption" by Greene et. al. (page: 7, fig: 3.4). */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define ROUNDS 16

#include "my_lib.h"
#include "oracle.h"

int main(){
    /* inserting 128 bit message and 256 bit key */
    uint32_t *msg = insert128(0x0, 0x0, 0x0, 0x0);
    uint32_t *key = insert256(0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c, 0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c);

    oracle(msg, key);

    /* for printing purpose */
    printf("\ncip: ");
    print(msg, 128);
}
