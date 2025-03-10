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
│
├── aradi
│   ├── main.c
│   ├── oracle.h
│   ├── my_lib.h
│   └── README.md
│
├── baksheesh
│   ├── main.c
│   ├── oracle.h
│   ├── my_lib.h
│   └── README.md
│
├── craft
│   ├── main.c
│   ├── oracle.h
│   └── README.md
│
├── DES
│   ├── des.h
│   ├── look_up_tables.h
│   ├── main.c
│   └── README.md
│
├── future
│   ├── main.c
│   ├── make_mult_table.c
│   ├── my_lib.h
│   ├── oracle.h
│   └── README.md
│
├── Gift
│   ├── version-64
│   │   ├── main.c
│   │   ├── oracle.h
│   │   ├── my_lib.h
│   │   └── README.md
│   │
│   └── version-128
│       ├── main.c
│       ├── oracle.h
│       ├── my_lib.h
│       └── README.md
│
├── Klein
│   └── version-64
│       ├── main.rs
│       ├── oracle.rs
│       └── README.md
│
├── Midori
│   └── version-64
│       ├── main.c
│       ├── my_lib.h
│       ├── oracle.h
│       └── README.md
│
├── Piccolo
│   └── version-80
│       ├── main.c
│       ├── my_lib.h
│       ├── oracle.h
│       └── README.md
│
├── Present
│   └── version-80
│       ├── main.c
│       ├── my_lib.h
│       ├── oracle.h
│       └── README.md
│
├── Prince
│   ├── main.c
│   ├── my_lib.h
│   ├── oracle.h
│   └── README.md
│
├── Print
│   └── version-48
│       ├── main.c
│       ├── my_lib.h
│       ├── oracle.h
│       └── README.md
│
├── Rectangle
│   └── version-80
│       ├── main.c
│       ├── my_lib.h
│       ├── oracle.h
│       └── README.md
│
├── Simon
│   └── version-32-64
│       ├── main.c
│       ├── my_lib.h
│       ├── oracle.h
│       └── README.md
│
├── Speck
│   └── version-32-64
│       ├── main.c
│       ├── my_lib.h
│       ├── oracle.h
│       └── README.md
│
├── Skinny
│   ├── version-64-64
│   │   ├── main.c
│   │   ├── my_lib.h
│   │   ├── oracle.h
│   │   └── README.md
│   │
│   ├── version-64-128
│   │   ├── main.c
│   │   ├── my_lib.h
│   │   ├── oracle.h
│   │   └── README.md
│   │
│   └── version-64-192
│       ├── main.c
│       ├── oracle.h
│       ├── my_lib.h
│       └── README.md
│
├── Warp
│   ├── main.c
│   ├── my_lib.h
│   ├── oracle.h
│   └── README.md
│
└── README.md
````

Each cipher folder contains:
- The main code file(s) for that cipher.
- A README.md containing the original paper details, the test vectors of the corresponding cipher and usage details of the given code.
