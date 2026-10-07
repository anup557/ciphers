/* gist: prog takes the test vectors from the file test_vectors.txt and checks whether the cip matches with the original */
/* one or not. In the test_vector.txt file make sure that the key is written in (line-3)%5, plaintext is written in (line-4)%5 */
/* and cip is written in (line-5)%5 number line. */
/* ------------------------------------------------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "/home/anup/Dropbox/lit_survey/000_prog/others/necessary_files/my_lib.h"
#include "oracle.h"

#define line_size 256
#define state_size 128

/* msg start pos depends on from what position the msg is starting, like ommiting the "key:   " thing */
#define msg_start_pos 12


/* takes the charecter and returns the corresponding integer */
uint8_t get_val(uint8_t ch){
    /* if ch is a number between 0-9 */
    if ((ch>=48) && (ch <= 57)){
        return (ch - 48);
        }

    /* if ch is a number between A-F, here (+10) because a=10 */
    else if ((ch>=65) && (ch <= 70)){
        return (ch - 65 + 10);
        }

    /* if ch is a number between a-f, here (+10) because a=10 */
    else if ((ch>=97) && (ch <= 102)){
        return (ch - 97 + 10);
        }

    /* if the charecter is not a hex number */
    else{
        printf("enter a valid number in the test vector file.\n");
        abort();
        }
    }


/* returns the number corresponding to the "str" from the line */
void get_num(uint64_t *msg, uint8_t* line){
    /* ch idx is for the charecter position in the line */
    uint16_t ch_idx = 0;
    while((ch_idx < (state_size/4)) && (line[ch_idx] != '\n')){
        msg[ch_idx/16] = (msg[ch_idx/16]<<4) | (get_val(line[ch_idx])&0xf);
        ch_idx++;
        }}


int main(){
    char *filename = "test_vectors.txt";
    FILE *fp = fopen(filename, "r");

    char line[line_size];

    int i=0;
    uint64_t    *key,
                *msg,
                *cip;

    /* taking one line at a time */
    while(fgets(line, line_size, fp)){
        i++;
        /* if the line contains '\n', then continue with the next one */
        if (strlen(line) == 1){
            continue;
            }

        /* in the 1st and 2nd line comment, "----" and '\n' are there */ 
        if ((((i-1)%5) == 0) || (((i-2)%5) == 0)){
            continue;
            }

        /* the pos should be cip, msg and key. As cip is coming at last in the test vector file. So, if the cip */
        /* line appears then key and msg line has already passed. */
        /* for the cip */
        cip = mem_alloc(state_size);
        if (((i-5)%5) == 0){
            get_num(cip, &line[msg_start_pos]);

            /* storing original msg */
            uint64_t *original_msg = mem_alloc(state_size);
            copy(original_msg, msg, state_size);

            /* storing original key */
            uint64_t *original_key = mem_alloc(state_size);
            copy(original_key, key, state_size);

            /* calling the oracle with msg and key */
            oracle(msg, key);

            /* msg for test vector fail */
            if (check_eq(msg, cip, state_size) != 1){
                printf("test vector check failed!\n");
                printf("msg: ");    print(original_msg, state_size);
                printf("key: ");    print(original_key, state_size);
                printf("cip: ");    print(cip, state_size);
                printf("\n");

                abort();
                }
            continue;
            }

        /* for the msg */
        msg = mem_alloc(state_size);
        if (((i-4)%5) == 0){
            get_num(msg, &line[msg_start_pos]);
            continue;
            }

        /* for the key */
        key = mem_alloc(state_size);
        if (((i-3)%5) == 0){
            get_num(key, &line[msg_start_pos]);
            }
        }

    printf("all the test vectors satisfied.\n");
    fclose(fp);
    }

