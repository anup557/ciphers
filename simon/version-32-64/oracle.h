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
    uint64_t z[4] = {0x3e8958737d12b0e6, 0x23be4c2d477c985a, 0x2bdc0d262847e5b3, 0x36eb19781229cd0f};

    /* the key is saved as k[3]|k[2]|k[1]|k[0] */
    for (uint8_t i=0; i<m; i++){
        k[i] = (key[0]>>(16*i))&0xffff;
    }

    /* generating other round keys */
    for (uint8_t i=m; i<T; i++){
        uint16_t tmp = S(-3, k[i-1]);

        /* for simon-32/64 its always true */
        if (m == 4){
            tmp = tmp^k[i-3];
        }
        tmp = tmp^S(-1, tmp);

        /* extracting the constant bit, here 62-1 as len of each z[] is 62 */
        uint16_t cons_bit = ((z[j]>>(62-1 - ((i-m)%62)))&1);
        uint16_t cons = ~(cons_bit^(uint16_t)3);

        k[i] = (k[i-m])^tmp^cons;
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
    uint16_t *round_key = (uint16_t*)malloc(T*sizeof(uint16_t));
    key_expansion(round_key, key);

    /* for printing purpose */
    if (print_details == "true"){
        printf("******************************************************\n");
        printf("encryption function:\n");
        printf("******************************************************\n");
    }
    /* encryption function */
    for (uint8_t i=0; i<T; i++){
        uint16_t tmp = x;
        x = y ^ S(1, x) & S(8, x) ^ S(2, x) ^ round_key[i];
        y = tmp;

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


