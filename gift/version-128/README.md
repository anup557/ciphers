This folder contains implementations of Gift-128 cipher in C language.

## Paper Details
1. Subhadeep Banik, Sumit Kumar Pandey, Thomas Peyrin, Yu Sasaki, Siang Meng Sim, and Yosuke Todo
GIFT: A Small Present Towards Reaching the Limit of Lightweight Encryption (Full version)
Cryptographic Hardware and Embedded Systems - CHES 2017.

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Gift-128 Test Vector
The test vectors are taken from "https://github.com/giftcipher/gift/tree/master/implementations/test%20vector".
There is no test vectors given in the original paper.

1.  - **Message:** 00000000000000000000000000000000
    - **Master Key:** 00000000000000000000000000000000
    - **Ciphertext:** cd0bd738388ad3f668b15a36ceb6ff92

2.  - **Message:** fedcba9876543210fedcba9876543210
    - **Master Key:** fedcba9876543210fedcba9876543210
    - **Ciphertext:** 8422241a6dbf5a9346af468409ee0152

3.  - **Message:** e39c141fa57dba43f08a85b6a91f86c1
    - **Master Key:** d0f5c59a7700d3e799028fa9f90ad837
    - **Ciphertext:** 13ede67cbdcc3dbf400a62d6977265ea
