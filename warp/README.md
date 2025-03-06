This folder contains implementations of Warp cipher in C language.

## Paper Details
1. Subhadeep Banik, Zhenzhen Bao, Takanori Isobe, Hiroyasu Kubo, Fukang Liu, Kazuhiko Minematsu, Kosei Sakamoto, Nao Shibata, Maki Shigeri<br />
WARP : Revisiting GFN for Lightweight 128-bit Block Cipher
Selected Areas in Cryptography 2021.

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Warp Test Vector
The test vectors are taken from Appendix A (Table 6) of the paper.

1.  - **Message:** 0123456789abcdef fedcba9876543210 
    - **Master Key:** 0123456789abcdef fedcba9876543210
    - **Ciphertext:** d8a30754fdf5d925 ed23f9dfe8a0ec42

2.  - **Message:** ffeeddccbbaa9988 7766554433221100 
    - **Master Key:** 0123456789abcdef fedcba9876543210 
    - **Ciphertext:** c21bf8452dd7669b 26ee72829f46c329 

3.  - **Message:** 1e193d8021dcb798 aae6a5cf09ddc6fa 
    - **Master Key:** 7d3e90b7680c30ee f745a086f220dca0
    - **Ciphertext:** 64dd850ddca14652 413d4291f5993216
