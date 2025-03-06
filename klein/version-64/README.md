This folder contains implementations of KLEIN-64 cipher in rust language.

## Paper Details
1. Zheng Gong, Svetla Nikova, Yee Wei Law.
KLEIN: A New Family of Lightweight Block Ciphers.
RFID. Security and Privacy - 7th International Workshop, 2011.

## Usage
To run the oracle, simply run the following command in linux terminal or run the `main.rs` file.
````
rustc main.rs -o out
./out
````

## KLEIN-64 Test Vector
The test vectors are taken from Appendix A (Table 4) of the paper.

1.  - **Message:** FFFFFFFFFFFFFFFF
    - **Key:** 0000000000000000
    - **Ciphertext:** CDC0B51F14722BBE

2.  - **Message:** 0000000000000000
    - **Key:** FFFFFFFFFFFFFFFF
    - **Ciphertext:** 6456764E8602E154

3.  - **Message:** FFFFFFFFFFFFFFFF
    - **Key:** 1234567890ABCDEF
    - **Ciphertext:** 592356C4997176C8

4.  - **Message:** 1234567890ABCDEF
    - **Key:** 0000000000000000
    - **Ciphertext:** 629F9D6DFF95800E
