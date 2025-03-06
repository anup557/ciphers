/* If the print details is true then only all the state values will print */
/* char *print_details = "false"; */
char *print_details = "true";

uint16_t S(int8_t i, uint16_t msg){
    /* for positive left circular shift */
    if (i>0){
        return ((msg<<i)|(msg>>(16-i)));
    }
    else if (i == 0){
        return msg;
    }
    /* for negetive right circular shift */
    else {
        return ((msg>>(-i))|(msg<<(16-(-i))));
    }
}


void key_expansion(uint16_t *k, uint64_t *key){
    uint16_t *l = (uint16_t*)malloc((T+2)*sizeof(uint16_t));

    /* the key is saved as l[2]|l[1]|l[0]|k[0] */
    for (uint8_t i=0; i<m; i++){
        if (i == 0){
            k[i] = (key[0]>>(16*i))&0xffff;
        }
        else{
            l[i-1] = (key[0]>>(16*i))&0xffff;
        }
    }

    /* generating other round keys */
    for (uint8_t i=0; i<T-1; i++){
        l[i+m-1] = (k[i] + S(-ALPHA, l[i]))^i;
        k[i+1] = S(BETA, k[i]) ^ l[i+m-1];
    }

    /* for printing purpose */
    if (print_details == "true"){
        printf("********************************************************************************\n");
        printf("round keys:\n");
        printf("********************************************************************************\n");
        for (uint8_t i=0; i<T; i++){
            printf("for round %d:\t %x\n", i, k[i]);
        }

        printf("\n");
    }
}


void oracle(uint64_t *msg, uint64_t *key){
    /* extracting x and y  from the msg */
    uint16_t x = (msg[0]>>16)&0xffff;
    uint16_t y = msg[0]&0xffff;

    /* for generating the roung keys from the key expansion */
    uint16_t *k = (uint16_t*)malloc(T*sizeof(uint16_t));
    key_expansion(k, key);

    /* for printing purpose */
    if (print_details == "true"){
        printf("******************************************************\n");
        printf("encryption function:\n");
        printf("******************************************************\n");
    }
    /* encryption function */
    for (uint8_t i=0; i<T; i++){
        x = (S(-ALPHA, x) + y) ^ k[i];
        y = S(BETA, y) ^ x;

        /* for printing purpose */
        if (print_details == "true"){
            printf("for round %d: %x %x\n", i, x, y);
        }
    } 
    msg[0] = (((uint64_t)x)<<16) | (y&0xffff);

    /* for printing purpose */
    if (print_details == "true"){
        printf("******************************************************\n");
    }
}


