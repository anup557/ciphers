run the file aes_simd.cpp file with the following command:
    g++ -maes -O3 aes_simd.cpp -o out_simd && ./out_simd

Each AES encryption function takes 23-25 clock cycles in my thinkpad laptop to execute.
