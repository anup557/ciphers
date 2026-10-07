/* If the print details is true then only all the state values will print */
/* char *print_details = "false"; */
char *print_details = "true";

/* circular shifts within 16 and 32 bit */
#define S16(i,X) ((((uint16_t)X<<i)&0xffff)|((uint16_t)X>>(16-i)))
#define S32(i,X) (((uint32_t)X<<i)|((uint32_t)X>>(32-i)))

#define M(i,j,X,Y) (((uint64_t)(S32(i,Y)^S32(j,X)^X)<< 32)|((S32(i,Y)^X)&0xffffffff))
#define L(a,b,c,x,y) (((uint32_t)(x^S16(a,x)^S16(c,y))<<16)|((y^S16(a,y)^S16(b,x))&0xffff))

void key_expansion(uint32_t *K, uint32_t *k){
    for (uint8_t i=0; i<ROUNDS; i++){
        uint8_t j = i%2;

        /* storing round keys */
        k[4*i+3] = K[4*j+3]; k[4*i+2] = K[4*j+2]; k[4*i+1] = K[4*j+1]; k[4*i+0] = K[4*j+0];

        /* applying M function */
        uint64_t tmp = M(1, 3, K[1], K[0]);
        K[1] = (tmp>>32)&0xffffffff;   K[0] = tmp&0xffffffff;

        tmp = M(9, 28, K[3], K[2]);
        K[3] = (tmp>>32)&0xffffffff;   K[2] = tmp&0xffffffff;

        tmp = M(1, 3, K[5], K[4]);
        K[5] = (tmp>>32)&0xffffffff;   K[4] = tmp&0xffffffff;

        tmp = M(9, 28, K[7], K[6]);
        K[7] = (tmp>>32)&0xffffffff;   K[6] = tmp&0xffffffff;
        K[7] = K[7]^i;

        /* word permutation */
        if (j == 0){
            uint32_t T = K[1]; K[1] = K[2]; K[2] = T;
            T = K[5]; K[5] = K[6]; K[6] = T;
        }
        else{
            uint32_t T = K[1]; K[1] = K[4]; K[4] = T;
            T = K[3]; K[3] = K[6]; K[6] = T;
        }
    }

    /* storing post round key */
    k[4*ROUNDS+3] = K[3];   k[4*ROUNDS+2] = K[2];
    k[4*ROUNDS+1] = K[1];   k[4*ROUNDS+0] = K[0];

    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        printf("round keys:\n");
        printf("********************************************************************************\n");
        for (uint8_t i=0; i<=ROUNDS; i++){
            printf("for round %d:\t%x %x %x %x\n", i, k[4*i], k[4*i+1], k[4*i+2], k[4*i+3]);
        }

        printf("\n");
    }
}


void oracle(uint32_t *msg, uint32_t *key){
    uint8_t a[4] = {11,10,9,8};
    uint8_t b[4] = {8,9,4,9};
    uint8_t c[4] = {14,11,14,7};

    uint32_t *k = malloc(4*(ROUNDS+1)*sizeof(uint32_t));

    key_expansion(key, k);

    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        printf("intermediate rounds:\n");
        printf("********************************************************************************\n");
    }

    for (uint8_t i=0; i<ROUNDS; i++){
        /* round key addition */
        msg[3] = msg[3]^k[4*i+3];   msg[2] = msg[2]^k[4*i+2];
        msg[1] = msg[1]^k[4*i+1];   msg[0] = msg[0]^k[4*i+0];

        /* sbox layer */
        msg[1] = msg[1]^(msg[0]&msg[2]);
        msg[3] = msg[3]^(msg[1]&msg[2]);
        msg[2] = msg[2]^(msg[0]&msg[3]);
        msg[0] = msg[0]^(msg[1]&msg[3]);

        uint8_t j = i%4;
        /* applying linear map */
        msg[3] = L(a[j],b[j],c[j],((msg[3]>>16)&0xffff), (msg[3]&0xffff));
        msg[2] = L(a[j],b[j],c[j],((msg[2]>>16)&0xffff), (msg[2]&0xffff));
        msg[1] = L(a[j],b[j],c[j],((msg[1]>>16)&0xffff), (msg[1]&0xffff));
        msg[0] = L(a[j],b[j],c[j],((msg[0]>>16)&0xffff), (msg[0]&0xffff));

        /* for printing purpose */
        if (print_details == "true"){
            printf("round %d:\t%x %x %x %x\n", i, msg[0], msg[1], msg[2], msg[3]);
        }
    }
    /* post key addition */
    msg[3] = msg[3]^k[4*ROUNDS+3]; msg[2] = msg[2]^k[4*ROUNDS+2];
    msg[1] = msg[1]^k[4*ROUNDS+1]; msg[0] = msg[0]^k[4*ROUNDS+0];

    free(k);

    /* for printing purpose */
    if (print_details == "true"){
        printf("after post-key:\t%x %x %x %x\n", msg[0], msg[1], msg[2], msg[3]);
    }
}
