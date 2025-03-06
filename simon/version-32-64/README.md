This folder contains implementations of SIMON-32/64 cipher in C language.

## Paper Details
1. Ray Beaulieu, Douglas Shors, Jason Smith, Stefan Treatman-Clark, Bryan Weeks, Louis Wingers.
The Simon and Speck families of Lightweight Block Ciphers.
Proceedings of the 52nd Annual Design Automation Conference 2015.

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## SIMON-32/64 Test Vector
The test vectors are taken from Appendix B of the paper.

1.  - **Message:** 6565 6877
    - **Master Key:** 1918 1110 0908 0100
    - **Ciphertext:** c69b e9bb
