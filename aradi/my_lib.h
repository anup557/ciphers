/* gist: This is my library functions for c programming. The functions works on the chunks of 64-bit */ 
/* memory. */
/* ----------------------------------------------------------------------------------------- */

/* functions: */
/*     (i)     uint32_t* mem_alloc(uint32_t size) */
/*     (ii)    void rand_alloc(uint32_t *msg, uint32_t size) */
/*     (iii)   void copy(uint32_t *x, uint32_t *y, uint32_t size) */    
/*     (iv)    void print(uint32_t *state, uint32_t size) */
/*     (v)     void circ_shift(uint32_t *msg, uint8_t pos, uint32_t size, char* str) */
/*     (vi)    void shift(uint32_t *msg, uint8_t pos, uint32_t size, char* str) */
/*     (vii)   void xr(uint32_t *a, uint32_t *b, uint32_t size) */
/*     (viii)  void insert(uint32_t *x, uint32_t msb, uint32_t lsb) */    
/*     (ix)    uint8_t check_eq(uint32_t *msg1, uint32_t *msg2, uint32_t size) */

/* ----------------------------------------------------------------------------------------- */


/* allocate the memory of 64-bit chunks */
uint32_t* mem_alloc(uint32_t size){
    uint32_t *msg = calloc((size/32), sizeof(uint32_t));
    return msg;
}


/* coping 32 bit chunks of y to x */
void copy(uint32_t *x, uint32_t *y, uint32_t size){    
    for (uint8_t i=0; i<(size/32); i++){
        x[i] = y[i];
        }
    }   


/* printing the size/4 many eles of the state */
void print(uint32_t *state, uint32_t size){
    for (uint8_t i=0; i<(size/32); i++){
        printf("%x ", state[i]);
        }   
    printf("\n");
    }


/* printing the size/4 many eles of the state */
void print_matrix(uint32_t *state){
    for (uint8_t row=0; row<4; row++){
        for (uint8_t col=0; col<4; col++){
            printf("%8x\t", state[4*row + col]);
        }
        printf("\n");
    }
}


/* /1* right circular shift for 128-bit only *1/ */
/* void circ_shift(uint32_t *msg, uint8_t pos, uint32_t size, char* str){ */
/*     /1* if the shifting position is greater than the msg size *1/ */
/*     if (pos>size){ */
/*         printf("shifting position is greater than msg size!"); */
/*         abort(); */
/*         } */

/*     /1* for the left shift *1/ */
/*     if (str == "left"){ */
/*         uint16_t idx64 = pos/64; */
/*         uint16_t pos64 = pos%64; */

/*         uint32_t *cip = mem_alloc(size); */

/*         for (uint16_t i=0; i<size/64; i++){ */
/*             /1* if (i+idx64) exceeds the cell size, then it will circularly takes the cell values from 0th cell *1/ */
/*             if ((i + idx64) > ((size/64) - 1)){ */
/*                 cip[i] = msg[i+idx64 - (size/64)]; */

/*                 /1* if the shifting in teh cell is non-zero *1/ */
/*                 if (pos64 != 0){ */
/*                     cip[i] = cip[i]<<pos64; */
/*                     cip[i] |= (msg[i+idx64 - ((size/64) -1)] >> (64-pos64)); */
/*                     } */
/*                 continue; */
/*                 } */

/*             /1* i+idx64 th cell will be copied to ith cell *1/ */
/*             cip[i] = msg[i + idx64]; */

/*             /1* if the shifting val is non-zero within the cell, then shift in the cell *1/ */
/*             if (pos64 != 0){ */
/*                 cip[i] = cip[i]<<pos64; */

/*                 /1* if the next 64-bit is available then shift in the next also *1/ */
/*                 if ((i + idx64) < ((size/64) - 1)){ */
/*                     cip[i] |= (msg[i+1+idx64] >> (64-pos64)); */
/*                     } */

/*                 /1* if the (i+idx64) reaches its limit, it will take the bits from the 0th cell *1/ */ 
/*                 if ((i + idx64) == ((size/64) - 1)){ */
/*                     cip[i] |= (msg[0] >> (64-pos64)); */
/*                     }}} */
/*         copy(msg, cip, size); */
/*         free(cip); */
/*         } */

/*     /1* for the right shift *1/ */
/*     else if (str == "right"){ */
/*         uint16_t idx64 = pos/64; */
/*         uint16_t pos64 = pos%64; */

/*         uint32_t *cip = mem_alloc(size); */

/*         for (int16_t i=size/64 - 1; i>=0; i--){ */
/*             /1* for the circular shift if the target cell is lower than 0, then continue with the last cell *1/ */  
/*             if ((i - idx64) < 0){ */
/*                 cip[i] = msg[(size/64)-1 - i]; */

/*                 /1* if there is nothing to shift in the cell *1/ */
/*                 if (pos64 != 0){ */
/*                     cip[i] = cip[i]>>pos64; */
/*                     cip[i] |= (msg[size/64 - 1 - (idx64 - i)] << (64-pos64)); */
/*                     } */

/*                 continue; */
/*                 } */

/*             cip[i] = msg[i-idx64]; */
/*             if (pos64 != 0){ */
/*                 cip[i] = cip[i] >> pos64; */

/*                 /1* if the next 64-bit is available then shift in the next also *1/ */
/*                 if ((i-idx64) > 0){ */
/*                     cip[i] |= (msg[i-idx64 -1] << (64-pos64)); */
/*                     } */

/*                 /1* if (i-idx64) reaches at 0, then the next will be the last cell *1/ */
/*                 if ((i - idx64) == 0){ */
/*                     cip[i] |= (msg[size/64 - 1] << (64-pos64)); */
/*                     }}} */

/*         copy(msg, cip, size); */
/*         free(cip); */
/*         } */

/*     /1* if the size is not given properly. *1/ */
/*     else{ */
/*         printf("the shift direction is not properly given.\n"); */
/*         abort(); */
/*         }} */


/* /1* depending upon the direction, msg shifts to pos many bits *1/ */
/* void shift(uint32_t *msg, uint8_t pos, uint32_t size, char* str){ */
/*     /1* for the left shift *1/ */
/*     if (str == "left"){ */
/*         uint16_t idx64 = pos/64; */
/*         uint16_t pos64 = pos%64; */

/*         uint32_t *cip = mem_alloc(size); */

/*         for (uint16_t i=0; i<size/64; i++){ */
/*             /1* if the cell exceeds the last cell then break *1/ */
/*             if ((i + idx64) >= size/64){ */
/*                 break; */
/*                 } */

/*             cip[i] = msg[i + idx64]; */

/*             /1* if the pos in the cell is not 0 *1/ */
/*             if (pos64 != 0){ */
/*                 cip[i] = cip[i]<<pos64; */

/*                 /1* if the next 64-bit is available then shift in the next also *1/ */
/*                 if ((i + idx64) < (size/64 - 1)){ */
/*                     cip[i] |= (msg[i+1+idx64] >> (64-pos64)); */
/*                     }}} */
/*         copy(msg, cip, size); */
/*         free(cip); */
/*         } */

/*     /1* for the right shift *1/ */
/*     else if (str == "right"){ */
/*         uint16_t idx64 = pos/64; */
/*         uint16_t pos64 = pos%64; */

/*         uint32_t *cip = mem_alloc(size); */

/*         for (int16_t i=size/64 - 1; i>=0; i--){ */
/*             /1* if the lowest cell exceeds 0th cell, then break *1/ */
/*             if ((i - idx64) < 0){ */
/*                 break; */
/*                 } */

/*             cip[i] = msg[i-idx64]; */

/*             /1* if the shifting pos in the cell is 0 *1/ */
/*             if (pos64 != 0){ */
/*                 cip[i] = cip[i]>>pos64; */

/*                 /1* for the cell numbers greater than 0 *1/ */
/*                 if ((i - idx64) >= 1){ */
/*                     cip[i] |= (msg[i-1-idx64] << (64-pos64)); */
/*                     }}} */
/*         copy(msg, cip, size); */
/*         free(cip); */
/*         } */

/*     else{ */
/*         printf("shift direction is not properly given.\n"); */
/*         abort(); */
/*         }} */


/* does xor of a, b (depending upon the size) and keep that in c */
void xr(uint32_t *a, uint32_t *b, uint32_t size){
    for (uint8_t i=0; i<(size/32); i++){
        a[i] ^= b[i];
    }
}


/* insert 128-bit in x */
uint32_t* insert128(uint32_t a, uint32_t b, uint32_t c, uint32_t d){    
    uint32_t *x = malloc(4*sizeof(uint32_t));
    x[0] = a; x[1] = b;
    x[2] = c; x[3] = d;

    return x;
}


/* insert 256-bit in x */
uint32_t* insert256(uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3,
                uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7){
    uint32_t *x = malloc(8*sizeof(uint32_t));

    x[0] = x0; x[1] = x1; x[2] = x2; x[3] = x3;
    x[4] = x4; x[5] = x5; x[6] = x6; x[7] = x7;

    return x;
}


/* /1* checks whether the two msgs are equal or not *1/ */
/* uint8_t check_eq(uint32_t *msg1, uint32_t *msg2, uint32_t size){ */
/*     for (uint8_t i=0; i<(size/64); i++){ */
/*         if (msg1[i] != msg2[i]){ */
/*             return 0; */
/*             }} */

/*     return 1; */
/*     } */   



