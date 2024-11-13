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

<!-- ### Round Key -->
<!-- | Round | Round Key | -->
<!-- |-------|--------------| -->
<!-- |0 | 03020100 07060504 0b0a0908 0f0e0d0c| -->
<!-- |1 | 31323734 2b2c2d2a 89829f94 eaddccfb| -->
<!-- |2 | 19181312 49484342 bfb2b5b8 efe2e5e8| -->
<!-- |3 | 93d8dd96 49bbf102 12918d0e 2caf0292| -->
<!-- |4 | 7c795e5b 6e0a4a2f 708952ab 0fb51eb7| -->
<!-- |5 | 73be37f3 b12de15c 6d10261a 63fa1fb1| -->
<!-- |6 | 30e1a565 56518eba 38a4dc70 43b62b6b| -->
<!-- |7 | 6ff94bf4 a1525d49 960d690a f40ac5e6| -->
<!-- |8 | 652b43fa 7ea0caa1 8356eca6 eed8d0ca| -->
<!-- |9 | 1e8816b8 eaf40402 bf1911db d2ed83c3| -->
<!-- |10 | 2aed0767 d7e42972 0ddcac43 e0ce34bd| -->
<!-- |11 | e587db6f d93a728e e7a79043 54e47c4c| -->
<!-- |12 | 5deafddf 1235c451 b9420597 1bc4fb83| -->
<!-- |13 | f95881fc a9cbae8e 266a00c2 64230546| -->
<!-- |14 | cc0fab2e 5b7aad77 32495539 b022810a| -->
<!-- |15 | 71c5c046 8ab9aa02 d8fb0856 b7dfa119| -->
<!-- |16 | a443053b 69322a8e e8abfb4f 41cf0ca8| -->
