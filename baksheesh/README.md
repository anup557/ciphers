This folder contains implementations of BAKSHEESH cipher in C language.

## Paper Details
1. 	Anubhab Baksi, Jakub Breier, Anupam Chattopadhyay, Tomas Gerlich, Sylvain Guilley, Naina Gupta, Kai Hu, Takanori Isobe, Arpan Jati, Petr Jedlicka, Hyunjun Kim, Fukang Liu, Zdenek Martinasek, Kosei Sakamoto, Hwajeong Seo, Rentaro Shiba, Ritu Ranjan Shrivastwa:
BAKSHEESH: Similar Yet Different From GIFT.
IACR Cryptol. ePrint Arch. 2023: 750 (2023)

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.c` file.
````
gcc main.c -o out
./out
````

## BAKSHEESH Test Vector
The following test vectors are taken from Table 14 of [1].

1.  - **Message:** 00000000000000000000000000000000
    - **Key:** 00000000000000000000000000000000
    - **Ciphertext:** c002be5e64c78a72ab9a3439518352aa

2.  - **Message:** 00000000000000000000000000000007
    - **Key:** 00000000000000000000000000000000
    - **Ciphertext:** 6f7d7746eaf0d97a154079f6bd846438

3.  - **Message:** 70000000000000000000000000000000
    - **Key:** 00000000000000000000000000000000
    - **Ciphertext:** 1ba3363734c09a29f67c23bbb2cccc05

4.  - **Message:** 44444444444444444444444444444444
    - **Key:** 00000000000000000000000000000000
    - **Ciphertext:** 7ad3303667b2af6deef434dd110d7fb8

5.  - **Message:** 11111111111111111111111111111111
    - **Key:** ffffffffffffffffffffffffffffffff
    - **Ciphertext:** 806f0cf45b94f0370206975fe78ac10f

6.  - **Message:** 789a789a789a789a789a789a789a789a
    - **Key:** 76543210032032032032032032032032
    - **Ciphertext:** ae654b5333b876584f8e8dd54f4e490a

7.  - **Message:** b6e4789ab6e4789ab6e4789ab6e4789a
    - **Key:** 23023023023023023023023001234567
    - **Ciphertext:** 3dbbdf7fe254cc0be396a753442dccad

8.  - **Message:** e6517531abf63f3d7805e126943a081c
    - **Key:** 5920effb52bc61e33a98425321e76915
    - **Ciphertext:** fc7e61fee3d587308ca7bc594ebf3244
