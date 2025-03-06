This folder contains implementations of Present-80 cipher in C language.

## Paper Details
1. A. Bogdanov, L.R. Knudsen, G. Leander, C. Paar, A. Poschmann, M.J.B. Robshaw, Y. Seurin, C. Vikkelsoe
PRESENT: An Ultra-Lightweight Block Cipher
Cryptographic Hardware and Embedded Systems - CHES 2007.

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Present-80 Test Vector
The test vectors are taken from Appendix I of the paper.

1.  - **Message:** 00000000 00000000
    - **Master Key:** 00000000 00000000 0000
    - **Ciphertext:** 5579C138 7B228445

2.  - **Message:** 00000000 00000000
    - **Master Key:** FFFFFFFF FFFFFFFF FFFF
    - **Ciphertext:** E72C46C0 F5945049

3.  - **Message:** FFFFFFFF FFFFFFFF
    - **Master Key:** 00000000 00000000 0000
    - **Ciphertext:** A112FFC7 2F68417B

4.  - **Message:** FFFFFFFF FFFFFFFF
    - **Master Key:** FFFFFFFF FFFFFFFF FFFF
    - **Ciphertext:** 3333DCD3 213210D2
