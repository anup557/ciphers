/* gist: code for piccolo-80 cipher in C. */
/* ------------------------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STATE_SIZE 64
/* for piccolo-80 key size is 80, but in that case also we are storing it in 128 bits */
#define KEY_SIZE 128

/* number of round for piccolo-80 is 25 */
#define NO_OF_ROUNDS 25

#include "my_lib.h"
#include "oracle.h"

int main(){
    uint64_t *msg = mem_alloc(STATE_SIZE);
    uint64_t *key = mem_alloc(KEY_SIZE);

    /* test vectors */
    msg[0] = 0x0123456789abcdef;
    insert(key, 0x0011, 0x2233445566778899);

    oracle(msg, key);

    /* for printing purpose */
    printf("\n\ncip: ");
    print(msg, STATE_SIZE);
    }   






