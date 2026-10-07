/* gist: This is my library functions for c programming. The functions works on the chunks of 32-bit */ 
/* memory. */
/* ------------------------------------------------------------------------------------------ */

/* allocate the memory of 64-bit chunks */
uint32_t* mem_alloc(uint32_t size){
    uint32_t *msg = calloc((size/32), sizeof(uint32_t));
    return msg;
}


/* printing the size/4 many eles of the state */
void print(uint32_t *state, uint32_t size){
    for (uint8_t i=0; i<(size/32); i++){
        printf("%x ", state[i]);
        }   
    printf("\n");
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
