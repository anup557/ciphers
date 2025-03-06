This folder contains implementations of Rectangle-80 cipher in C language.

## Paper Details
1. Wentao Zhang, Zhenzhen Bao, Dongdai Lin, Vincent Rijmen, Bohan Yang, Ingrid Verbauwhede
RECTANGLE: A Bit-slice Lightweight Block Cipher Suitable for Multiple Platforms
Science China Information Sciences 2015. 

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Rectangle-80 Test Vector
The test vectors are taken from Table 10 of the paper.

1.  - **Message:** 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000
    - **Master Key:** 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000
    - **Ciphertext:** 0010110110010110 1110001101010100 1110100010110001 0000100001110100 (in bit) = 2d96 e354 e8b1 0874 (in hex)

2.  - **Message:** 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111
    - **Master Key:** 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111
    - **Ciphertext:** 1001100101000101 1010101000110100 1010111000111101 0000000100010010 (in bit) = 9945 aa34 ae3d 0112 (in hex)
