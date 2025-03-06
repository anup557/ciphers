This folder contains implementations of Skinny-64-192 cipher in C language.

## Paper Details
1. Christof Beierle, Jérémy Jean, Stefan Kölbl, Gregor Leander, Amir Moradi, Thomas Peyrin, Yu Sasaki, Pascal Sasdrich, Siang Meng Sim
The SKINNY Family of Block Ciphers and its Low-Latency Variant MANTIS
Advances in Cryptology - CRYPTO 2016.

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Skinny-64-192 Test Vector
The test vectors are taken from Appendix B.1 of the paper.

1.  - **Message:** 530c61d35e8663c3
    - **Master Key:** ed00c85b120d6861 8753e24bfd908f60 b2dbb41b422dfcd0
    - **Ciphertext:** dd2cf1a8f330303c
