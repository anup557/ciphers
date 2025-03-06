This folder contains implementations of Speck-32/64 cipher in C language.

## Paper Details
1. Ray Beaulieu, Douglas Shors, Jason Smith, Stefan Treatman-Clark, Bryan Weeks, Louis Wingers
The Simon and Speck Families of Lightweight Block Ciphers
Proceedings of the 52nd Annual Design Automation Conference 2015.

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Speck-32/64 Test Vector
The test vectors are taken from Appendix C of the paper.

1.  - **Message:** 6574 694c
    - **Master Key:** 1918 1110 0908 0100
    - **Ciphertext:** a868 42f2
