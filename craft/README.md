This folder contains implementations of Craft cipher in C language.

## Paper Details
1. Christof Beierle, Gregor Leander, Amir Moradi, Shahram Rasoolzadeh.
CRAFT: Lightweight Tweakable Block Cipher with Efficient Protection Against DFA Attacks.
IACR Trans. Symmetric Cryptol. 2019(1): 5-45 (2019).

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Craft Test Vector
The test vectors are taken from Appendix A of [1].

1.  - **Message:** 0000000000000000
    - **K0:** 0000000000000000
    - **K1:** 0000000000000000
    - **T:** 0000000000000000
    - **Ciphertext:** F630538883063100

2.  - **Message:** 0123456789ABCDEF
    - **K0:** 0000000000000000
    - **K1:** 0000000000000000
    - **T:** 0123456789ABCDEF
    - **Ciphertext:** 2DB468477C1D6C59

3.  - **Message:** FEDCBA9876543210
    - **K0:** 0123456789ABCDEF
    - **K1:** FEDCBA9876543210
    - **T:** 0000000000000000
    - **Ciphertext:** 9EDA9131B9155B51

4.  - **Message:** 5734F006D8D88A3E
    - **K0:** 27A6781A43F364BC
    - **K1:** 916708D5FBB5AEFE
    - **T:** 54CD94FFD0670A58
    - **Ciphertext:** A17D6BD4BEEB996F
