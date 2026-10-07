This folder contains implementations of aradi cipher in C language.

## Paper Details
1. Patricia Greene, Mark Motley, Bryan Weeks.
ARADI and LLAMA: Low-Latency Cryptography for Memory Encryption.
IACR Cryptol. ePrint Arch. 2024: 1240 (2024).

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Aradi Test Vector
The following text vector is given in Figure 3.4 of [1].

- **Message:** 00000000 00000000 00000000 00000000
- **Key:** 03020100 07060504 0b0a0908 0f0e0d0c 13121110 17161514 1b1a1918 1f1e1d1c
- **Ciphertext:** 3f09abf4 00e3bd74 03260def b7c53912
