This folder contains implementations of Prince cipher in C language.

## Paper Details
1. Julia Borghoff, Anne Canteaut, Tim Güneysu, Elif Bilge Kavun, Miroslav Knežević, Lars R. Knudsen, Gregor Leander,
Ventzislav Nikov, Christof Paar, Christian Rechberger, Peter Rombouts, Søren S. Thomsen, Tolga Yalçın.
PRINCE – A Low-latency Block Cipher for Pervasive Computing Applications.
Advances in Cryptology - ASIACRYPT 2012. 

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## Prince Test Vector
The test vectors are taken from Appendix A of the paper.

1.  - **Message:** 0000000000000000 
    - **k0:** 0000000000000000 
    - **k1:** 0000000000000000 
    - **Ciphertext:** 818665aa0d02dfda

2.  - **Message:** ffffffffffffffff
    - **k0:** 0000000000000000
    - **k1:** 0000000000000000 
    - **Ciphertext:** 604ae6ca03c20ada

3.  - **Message:** 0000000000000000 
    - **k0:** ffffffffffffffff
    - **k1:** 0000000000000000 
    - **Ciphertext:** 9fb51935fc3df524

4.  - **Message:** 0000000000000000 
    - **k0:** 0000000000000000 
    - **k1:** ffffffffffffffff
    - **Ciphertext:** 78a54cbe737bb7ef

5.  - **Message:** 0123456789abcdef
    - **k0:** 0000000000000000
    - **k1:** fedcba9876543210
    - **Ciphertext:** ae25ad3ca8fa9ccf
