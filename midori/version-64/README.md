This folder contains implementations of Midori-64 cipher in C language.

## Paper Details
1. Subhadeep Banik, Andrey Bogdanov, Takanori Isobe, Kyoji Shibutani, Harunaga Hiwatari, Toru Akishita, Francesco Regazzoni
Midori: A Block Cipher for Low Energy (Extended Version)
Advances in Cryptology – ASIACRYPT 2015. 

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Midori-64 Test Vector
The test vectors are taken from Appendix A of the paper.

1.  - **Message:** 0000000000000000
    - **Master Key:** 00000000000000000000000000000000
    - **Ciphertext:** 3c9cceda2bbd449a

2.  - **Message:** 42c20fd3b586879e
    - **Master Key:** 687ded3b3c85b3f35b1009863e2a8cbf
    - **Ciphertext:** 66bcdc6270d901cd
