This folder contains implementations of Skinny-64-64 cipher in C language.

## Paper Details
1. Christof Beierle, Jérémy Jean, Stefan Kölbl, Gregor Leander, Amir Moradi, Thomas Peyrin, Yu Sasaki, Pascal Sasdrich, Siang Meng Sim.
The SKINNY Family of Block Ciphers and its Low-Latency Variant MANTIS.
Advances in Cryptology - CRYPTO 2016.

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Skinny-64-64 Test Vector
The test vectors are taken from Appendix B.1 of the paper.

1.  - **Message:** 06034 f957724d19d
    - **Master Key:** f5269826fc681238
    - **Ciphertext:** bb39dfb2429b8ac7
