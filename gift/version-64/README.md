This folder contains implementations of Gift-64 cipher in C language.

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

## Gift-64 Test Vector
The test vectors are taken from "https://github.com/giftcipher/gift/tree/master/implementations/test%20vector".
There is no test vectors given in the original paper.

1.  - **Message:** 0000000000000000
    - **Master Key:** 00000000000000000000000000000000
    - **Ciphertext:** f62bc3ef34f775ac

2.  - **Message:** fedcba9876543210
    - **Master Key:** fedcba9876543210fedcba9876543210
    - **Ciphertext:** c1b71f66160ff587

3.  - **Message:** c450c7727a9b8a7d
    - **Master Key:** bd91731eb6bc2713a1f9f6ffc75044e7
    - **Ciphertext:** e3272885fa94ba8b
