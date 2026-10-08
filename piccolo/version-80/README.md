This folder contains implementations of Piccolo-80 cipher in C language.

## Paper Details
1. Kyoji Shibutani, Takanori Isobe, Harunaga Hiwatari, Atsushi Mitsuda, Toru Akishita, Taizo Shirai.
Piccolo: An Ultra-Lightweight Blockcipher.
Cryptographic Hardware and Embedded Systems - CHES 2011. 

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Piccolo-80 Test Vector
The test vectors are taken from Appendix A of the paper.

1.  - **Message:** 01234567 89abcdef
    - **Master Key:** 00112233 44556677 8899
    - **Ciphertext:** 8d2bff99 35f84056
