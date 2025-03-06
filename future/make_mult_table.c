/* gist: this progs gives the multipications within the field GF(2^4)/(X^4 + X + 1) */
/* ---------------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


/* multiplying two polynomials in gf(16) */
uint8_t gm(uint8_t a, uint8_t b){
    uint8_t g=0;
    uint8_t deg_poly = 4;

    /* the irr poly: x^4 + x + 1 */
    uint8_t gf_poly = 0x13;

    int i;
    for (int i=0; i<deg_poly; i++){
        if ((b&0x1 == 1)){
            g ^=a;
        }
        uint8_t hbs = (a&0x8);
        a = a<<0x1; 
        if (hbs == 0x8){
            a ^= gf_poly;
        }
        b = b>>0x1;
    }

    return g;
}


int main(){
    /* mult ele is the element whose multipication table we want to compute */
    uint8_t mult_ele = 0x4;

    printf("\nmultipication table of %d: ", mult_ele);
    for (uint8_t i=0; i<16; i++){
        printf("0x%x, ", gm(i, mult_ele));
    }
}




