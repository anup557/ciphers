This repository contains implementations of various cryptographic ciphers mostly in C language. Each folder corresponds to a different cipher. This collection is intended for educational purposes.

# Folder Structure
The folder structure is as follows:

<!-- ├, │, ─, └ to create tree in md language. Use that in code format (within {````, ````} so that it displays as it is) -->
````
.
├── aes
│   └── version-128
│       ├── main.c
│       ├── oracle.h
│       ├── my_lib.h
│       └── README.md
├── aradi
│   ├── main.c
│   ├── oracle.h
│   ├── my_lib.h
│   └── README.md
├── baksheesh
│   ├── main.c
│   ├── oracle.h
│   ├── my_lib.h
│   └── README.md
└── README.md
````

Each cipher folder contains:
- The main code file(s) for that cipher.
- A README.md containing the original paper details, the test vectors of the corresponding cipher and usage details of the given code.
