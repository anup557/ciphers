This folder contains implementations of Future cipher in C language.

## Paper Details
1. Kishan Chand Gupta, Sumit Kumar Pandey, Susanta Samanta
FUTURE: A Lightweight Block Cipher Using An Optimal Diffusion Matrix
Progress in Cryptology - AFRICACRYPT 2022.

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Future Test Vector
The test vectors are taken from Appendix A of the paper.

1.  - **Message:** 0000000000000000
    - **Master Key:** 00000000000000000000000000000000
    - **Ciphertext:** 298650c13199cdec

2.  - **Message:** 0000000000000000
    - **Master Key:** 00000000000000001111111111111111
    - **Ciphertext:** 4aa41b330751b83d

3.  - **Message:** ffffffffffffffff
    - **Master Key:** 00102030405060708090a0b0c0d0e0f
    - **Ciphertext:** 68e030733fe73b8a

4.  - **Message:**  ffffffffffffffff
    - **Master Key:** ffffffffffffffffffffffffffffffff
    - **Ciphertext:** 333ba4b7646e09f2

5.  - **Message:** 6162636465666768
    - **Master Key:** 00000000000000000000000000000000
    - **Ciphertext:** cc5ba5e52038b6df

6.  - **Message:** 5353414d414e5441
    - **Master Key:** 05192832010913645029387763948871
    - **Ciphertext:** 5ce1b8d8d01a9310
