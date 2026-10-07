#include <stdio.h>
#include <stdint.h>
#include <emmintrin.h> // Header for SSE2 intrinsics (__m128i)
#include <wmmintrin.h> // Header for AES-NI

#include "oracle.h"

int main() {
    /* initializing msg, key */
    uint8_t msg[] = {0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51};
    uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

    // 2. Load the 16 bytes from memory into a single 128-bit SIMD register (XMM)
    // "u" stands for unaligned memory access, which is safer if your array isn't 16-byte aligned.
    __m128i msg128 = _mm_loadu_si128((const __m128i*)msg);
    __m128i key128 = _mm_loadu_si128((const __m128i*)key);

    __m128i rnd_key[11];
    key_schedule(key128, rnd_key);

    __m128i cip128 = enc(msg128, rnd_key);
    uint8_t cip[16];
    _mm_storeu_si128((__m128i*)cip, cip128);

    // 4. Print the contents byte by byte
    printf("cip:\n");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", cip[i]);
    }
    printf("\n");

    return 0;
}
