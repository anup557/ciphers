# BEANIE Block Cipher Implementation in C++

This repository contains a high-performance C++ implementation of the **BEANIE** tweakable block cipher using SIMD (SSSE3) acceleration for the Tweak-Key Schedule (TKS).

---

## 📄 Reference Paper & Test Vector Location

* **Specification Paper:** *BEANIE – A 32-bit Cipher for Cryptographic Mitigations against Software Attacks*
* **Authors:** Simon Gerhalter, Samir Hodžić, Marcel Medwed, Marcel Nageler, Artur Folwarczny, Ventzi Nikov, Jan Hoogerbrugge, Tobias Schneider, Gary McConville, and Maria Eichlseder
* **Specification Location:** Section 3.1 (Data Path) & Section 3.2 (Tweak-Key Schedule - TKS)
* **Test Vectors Location:** **Appendix C (Test Vectors) & Table 15** in the specification paper

---

## 🧪 Test Vectors

### Test Vector 1 (Zero Test Vector - Currently Active in `main.cpp`)
* **Plaintext (32-bit):** `0x00000000`
* **Master Key (128-bit):** `0x00000000000000000000000000000000`
* **Tweak Key (128-bit):** `0x00000000000000000000000000000000`
* **Expected Ciphertext (32-bit):** `0xda46f4d3`

### Test Vector 2 (Non-Zero Test Vector - Appendix C, Table 15)
* **Plaintext (32-bit):** `0x1841938a`
* **Master Key (128-bit):** `0xbd9c9afe2626f233706ac764af470a53`
* **Tweak Key (128-bit):** `0x2518c65012c8cdfb84064a42a281c3aa`
* **Expected Ciphertext (32-bit):** `0x092c2fea`

---

## 📂 File Structure

```text
beanie/
├── main.cpp       # Main test application executing test vectors
├── oracle.h       # BEANIE encryption oracle and data path
├── tks.h          # SIMD-accelerated Tweak-Key Schedule (TKS) functions
├── key_schedule1.cpp # Standalone SIMD key schedule program
└── README.md      # Documentation, instructions, and test vectors
```

---

## 🚀 How to Build and Run

### Prerequisites
* A C++ compiler supporting C++11 or higher (`g++` or `clang++`).
* x86 CPU supporting SSSE3 instructions (`-mssse3`).

### 1. Compile `main.cpp`
```bash
g++ -O3 -mssse3 main.cpp -o beanie_app
```

### 2. Execute the binary
```bash
./beanie_app
```

---

### Expected Output for `main.cpp` (Test Vector 1)

```text
Plaintext:  0x00000000
Ciphertext: 0xda46f4d3
```

*(Note: In `main.cpp`, you can uncomment Test Vector 2 to test the non-zero test vector from Table 15).*
