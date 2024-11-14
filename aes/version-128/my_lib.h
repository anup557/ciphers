/* gist: This is my library functions for c programming. The functions works on the chunks of 64-bit */ 
/* memory. */
/* ----------------------------------------------------------------------------------------- */
/* allocate the memory of 64-bit chunks */
uint64_t* mem_alloc(uint32_t size){
    uint64_t *msg = (uint64_t*)calloc((size/64), sizeof(uint64_t));
    return msg;
}


/* this creates the random msg of the given size */
void rand_alloc(uint64_t *msg, uint32_t size){
    for (uint16_t i=0; i<(size/64); i++){
        msg[i] = rand();
        msg[i] = (msg[i]<<32)|(rand()&0xffffffff);
        }
    }   


/* coping 64-bits of y to x */
void copy(uint64_t *x, uint64_t *y, uint32_t size){    
    for (uint8_t i=0; i<(size/64); i++){
        x[i] = y[i];
        }
    }   


/* printing the size/4 many eles of the state */
void print(uint64_t *state, uint32_t size){
    for (uint8_t i=0; i<(size/64); i++){
        printf("%lx ", state[i]);
        }   
    printf("\n");
    }


/* right circular shift for 128-bit only */
void circ_shift(uint64_t *msg, uint8_t pos, uint32_t size, char* str){
    /* if the shifting position is greater than the msg size */
    if (pos>size){
        printf("shifting position is greater than msg size!");
        abort();
        }

    /* for the left shift */
    if (str == "left"){
        uint16_t idx64 = pos/64;
        uint16_t pos64 = pos%64;

        uint64_t *cip = mem_alloc(size);

        for (uint16_t i=0; i<size/64; i++){
            /* if (i+idx64) exceeds the cell size, then it will circularly takes the cell values from 0th cell */
            if ((i + idx64) > ((size/64) - 1)){
                cip[i] = msg[i+idx64 - (size/64)];

                /* if the shifting in teh cell is non-zero */
                if (pos64 != 0){
                    cip[i] = cip[i]<<pos64;
                    cip[i] |= (msg[i+idx64 - ((size/64) -1)] >> (64-pos64));
                    }
                continue;
                }

            /* i+idx64 th cell will be copied to ith cell */
            cip[i] = msg[i + idx64];

            /* if the shifting val is non-zero within the cell, then shift in the cell */
            if (pos64 != 0){
                cip[i] = cip[i]<<pos64;

                /* if the next 64-bit is available then shift in the next also */
                if ((i + idx64) < ((size/64) - 1)){
                    cip[i] |= (msg[i+1+idx64] >> (64-pos64));
                    }

                /* if the (i+idx64) reaches its limit, it will take the bits from the 0th cell */ 
                if ((i + idx64) == ((size/64) - 1)){
                    cip[i] |= (msg[0] >> (64-pos64));
                    }}}
        copy(msg, cip, size);
        free(cip);
        }

    /* for the right shift */
    else if (str == "right"){
        uint16_t idx64 = pos/64;
        uint16_t pos64 = pos%64;

        uint64_t *cip = mem_alloc(size);

        for (int16_t i=size/64 - 1; i>=0; i--){
            /* for the circular shift if the target cell is lower than 0, then continue with the last cell */  
            if ((i - idx64) < 0){
                cip[i] = msg[(size/64)-1 - i];

                /* if there is nothing to shift in the cell */
                if (pos64 != 0){
                    cip[i] = cip[i]>>pos64;
                    cip[i] |= (msg[size/64 - 1 - (idx64 - i)] << (64-pos64));
                    }

                continue;
                }

            cip[i] = msg[i-idx64];
            if (pos64 != 0){
                cip[i] = cip[i] >> pos64;

                /* if the next 64-bit is available then shift in the next also */
                if ((i-idx64) > 0){
                    cip[i] |= (msg[i-idx64 -1] << (64-pos64));
                    }

                /* if (i-idx64) reaches at 0, then the next will be the last cell */
                if ((i - idx64) == 0){
                    cip[i] |= (msg[size/64 - 1] << (64-pos64));
                    }}}

        copy(msg, cip, size);
        free(cip);
        }

    /* if the size is not given properly. */
    else{
        printf("the shift direction is not properly given.\n");
        abort();
        }}


/* depending upon the direction, msg shifts to pos many bits */
void shift(uint64_t *msg, uint8_t pos, uint32_t size, char* str){
    /* for the left shift */
    if (str == "left"){
        uint16_t idx64 = pos/64;
        uint16_t pos64 = pos%64;

        uint64_t *cip = mem_alloc(size);

        for (uint16_t i=0; i<size/64; i++){
            /* if the cell exceeds the last cell then break */
            if ((i + idx64) >= size/64){
                break;
                }

            cip[i] = msg[i + idx64];

            /* if the pos in the cell is not 0 */
            if (pos64 != 0){
                cip[i] = cip[i]<<pos64;

                /* if the next 64-bit is available then shift in the next also */
                if ((i + idx64) < (size/64 - 1)){
                    cip[i] |= (msg[i+1+idx64] >> (64-pos64));
                    }}}
        copy(msg, cip, size);
        free(cip);
        }

    /* for the right shift */
    else if (str == "right"){
        uint16_t idx64 = pos/64;
        uint16_t pos64 = pos%64;

        uint64_t *cip = mem_alloc(size);

        for (int16_t i=size/64 - 1; i>=0; i--){
            /* if the lowest cell exceeds 0th cell, then break */
            if ((i - idx64) < 0){
                break;
                }

            cip[i] = msg[i-idx64];

            /* if the shifting pos in the cell is 0 */
            if (pos64 != 0){
                cip[i] = cip[i]>>pos64;

                /* for the cell numbers greater than 0 */
                if ((i - idx64) >= 1){
                    cip[i] |= (msg[i-1-idx64] << (64-pos64));
                    }}}
        copy(msg, cip, size);
        free(cip);
        }

    else{
        printf("shift direction is not properly given.\n");
        abort();
        }}


/* does xor of a, b (depending upon the size) and keep that in c */
void xr(uint64_t *a, uint64_t *b, uint32_t size){
    for (uint8_t i=0; i<(size/64); i++){
        a[i] ^= b[i];
        }
    }


void or(uint64_t *a, uint64_t *b, uint32_t size){
    for (uint8_t i=0; i<(size/64); i++){
        a[i] |= b[i];
        }
    }


/* insert 128-bit in x */
void insert(uint64_t *x, uint64_t msb, uint64_t lsb){    
    x[0] = msb; x[1] = lsb;
    }   


/* checks whether the two msgs are equal or not */
uint8_t check_eq(uint64_t *msg1, uint64_t *msg2, uint32_t size){
    for (uint8_t i=0; i<(size/64); i++){
        if (msg1[i] != msg2[i]){
            return 0;
            }}

    return 1;
    }   



