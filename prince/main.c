/* gist: code for prince cipher in C. */
/* ------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STATE_SIZE 64
#define KEY_SIZE 128

#define NO_OF_ROUNDS 12

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc(STATE_SIZE);
    uint64_t *key = mem_alloc(KEY_SIZE);

    /* prince test vector given in the paper */
    /* key = k0||k1 */
    /* msg[0] = 0x0; */
    /* insert(key, 0x0, 0x0); */

    /* msg[0] = 0xffffffffffffffff; */
    /* insert(key, 0x0, 0x0); */

    /* msg[0] = 0x0; */
    /* insert(key, 0xffffffffffffffff, 0x0); */

    /* msg[0] = 0x0; */
    /* insert(key, 0x0, 0xffffffffffffffff); */

    msg[0] = 0x0123456789abcdef;
    insert(key, 0x0, 0xfedcba9876543210);

    oracle(msg, key);

    /* for printing purpose */
    printf("\ncip: ");
    print(msg, STATE_SIZE);

    /* printf("\n"); */
    /* print_state(msg); */
    }
