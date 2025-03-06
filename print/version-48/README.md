This folder contains implementations of Print-48 cipher in C language.

## Paper Details
1. Lars Knudsen, Gregor Leander, Axel Poschmann, Matthew J.B. Robshaw.
PRINTcipher: A Block Cipher for IC-Printing.
Cryptographic Hardware and Embedded Systems, CHES 2010. 

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Print-48 Test Vector
The test vectors are taken from Table 6 of the paper.

1.  - **Message:** 4C847555C35B
    - **Key:** C28895BA327B 
    - **Perm Key:** 69D2CDB6
    - **Ciphertext:** EB4AF95E7D37
