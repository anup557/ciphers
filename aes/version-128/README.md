This folder contains implementations of AES-128 cipher in C language.

## Paper Details
1. Joan Daemen, Vincent Rijmen:
The Design of Rijndael: AES - The Advanced Encryption Standard.
Information Security and Cryptography, Springer 2002, ISBN 3-540-42580-2

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## AES-128 Test Vector
The test vectors are taken from [here](https://github.com/ircmaxell/quality-checker/blob/master/tmp/gh_18/PHP-PasswordLib-master/test/Data/Vectors/aes-ecb.test-vectors). The LSB of the lower state values are in the right side.

1.  - **Message:** 54776F204F6E65204E696E652054776F
    - **Key:** 5468617473206D79204B756E67204675
    - **Ciphertext:** 29c3505f571420f6402299b31a02d73a

2.  - **Message:** 6bc1bee22e409f96e93d7e117393172a
    - **Key:** 2b7e151628aed2a6abf7158809cf4f3c
    - **Ciphertext:** 3ad77bb40d7a3660a89ecaf32466ef97

3.  - **Message:** ae2d8a571e03ac9c9eb76fac45af8e51
    - **Key:** 2b7e151628aed2a6abf7158809cf4f3c
    - **Ciphertext:** f5d3d58503b9699de785895a96fdbaaf
