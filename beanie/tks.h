#ifndef TKS_H
#define TKS_H

#include <cstdint>
#include <tmmintrin.h> // SSSE3 (_mm_shuffle_epi8)
#include <emmintrin.h> // SSE2

// 4-bit S-Box table in a 128-bit SIMD register
alignas(16) static const uint8_t SBOX128[16] = {
    0x0, 0x4, 0x2, 0xb, 0xa, 0xc, 0x9, 0x8,
    0x5, 0xf, 0xd, 0x3, 0x7, 0x1, 0x6, 0xe
};

// PRINCE Round Constants for TKS (stored as 64-bit word pairs for SIMD loading)
alignas(16) static const uint64_t RC128[10][2] = {
    {0x0ULL, 0x0000000000000000ULL},
    {0x13198a2e03707344ULL, 0x0ULL},
    {0xa4093822299f31d0ULL, 0x0ULL},
    {0x082efa98ec4e6c89ULL, 0x0ULL},
    {0x452821e638d01377ULL, 0x0ULL},
    {0xbe5466cf34e90c6cULL, 0x0ULL},
    {0x7ef84f78fd955cb1ULL, 0x0ULL},
    {0x85840851f1ac43aaULL, 0x0ULL},
    {0xc882d32f25323c54ULL, 0x0ULL},
    {0x64a51195e0e3610dULL, 0x0ULL}
};

// Parallel SIMD 4-bit S-Box substitution on 128-bit state using PSHUFB (_mm_shuffle_epi8)
static inline __m128i sbox(__m128i state, __m128i sbox_lut, __m128i mask4) {
    __m128i lo = _mm_and_si128(state, mask4);
    __m128i hi = _mm_and_si128(_mm_srli_epi16(state, 4), mask4);

    __m128i sbox_lo = _mm_shuffle_epi8(sbox_lut, lo);
    __m128i sbox_hi = _mm_shuffle_epi8(sbox_lut, hi);

    return _mm_or_si128(sbox_lo, _mm_slli_epi16(sbox_hi, 4));
}

// Pure SIMD PRINCE M Matrix linear layer operating directly on __m128i
static inline __m128i prince_m(__m128i state) {
    __m128i mask_n0 = _mm_set1_epi16(0x000F);
    __m128i mask_n1 = _mm_set1_epi16(0x00F0);
    __m128i mask_n2 = _mm_set1_epi16(0x0F00);
    __m128i mask_n3 = _mm_set1_epi16(0xF000);

    __m128i n0 = _mm_and_si128(state, mask_n0);
    __m128i n1 = _mm_srli_epi16(_mm_and_si128(state, mask_n1), 4);
    __m128i n2 = _mm_srli_epi16(_mm_and_si128(state, mask_n2), 8);
    __m128i n3 = _mm_srli_epi16(_mm_and_si128(state, mask_n3), 12);

    __m128i mask_m0 = _mm_set1_epi64x(0xFFFF00000000FFFFULL);

    __m128i m7 = _mm_set1_epi16(0x7);
    __m128i mB = _mm_set1_epi16(0xB);
    __m128i mD = _mm_set1_epi16(0xD);
    __m128i mE = _mm_set1_epi16(0xE);

    __m128i m0_out0 = _mm_xor_si128(_mm_xor_si128(_mm_and_si128(n3, mE), _mm_and_si128(n2, m7)),
                                    _mm_xor_si128(_mm_and_si128(n1, mB), _mm_and_si128(n0, mD)));

    __m128i m0_out1 = _mm_xor_si128(_mm_xor_si128(_mm_and_si128(n3, mD), _mm_and_si128(n2, mE)),
                                    _mm_xor_si128(_mm_and_si128(n1, m7), _mm_and_si128(n0, mB)));

    __m128i m0_out2 = _mm_xor_si128(_mm_xor_si128(_mm_and_si128(n3, mB), _mm_and_si128(n2, mD)),
                                    _mm_xor_si128(_mm_and_si128(n1, mE), _mm_and_si128(n0, m7)));

    __m128i m0_out3 = _mm_xor_si128(_mm_xor_si128(_mm_and_si128(n3, m7), _mm_and_si128(n2, mB)),
                                    _mm_xor_si128(_mm_and_si128(n1, mD), _mm_and_si128(n0, mE)));

    __m128i m1_out0 = _mm_xor_si128(_mm_xor_si128(_mm_and_si128(n3, m7), _mm_and_si128(n2, mB)),
                                    _mm_xor_si128(_mm_and_si128(n1, mD), _mm_and_si128(n0, mE)));

    __m128i m1_out1 = _mm_xor_si128(_mm_xor_si128(_mm_and_si128(n3, mE), _mm_and_si128(n2, m7)),
                                    _mm_xor_si128(_mm_and_si128(n1, mB), _mm_and_si128(n0, mD)));

    __m128i m1_out2 = _mm_xor_si128(_mm_xor_si128(_mm_and_si128(n3, mD), _mm_and_si128(n2, mE)),
                                    _mm_xor_si128(_mm_and_si128(n1, m7), _mm_and_si128(n0, mB)));

    __m128i m1_out3 = _mm_xor_si128(_mm_xor_si128(_mm_and_si128(n3, mB), _mm_and_si128(n2, mD)),
                                    _mm_xor_si128(_mm_and_si128(n1, mE), _mm_and_si128(n0, m7)));

    __m128i out0 = _mm_or_si128(_mm_and_si128(mask_m0, m0_out0), _mm_andnot_si128(mask_m0, m1_out0));
    __m128i out1 = _mm_or_si128(_mm_and_si128(mask_m0, m0_out1), _mm_andnot_si128(mask_m0, m1_out1));
    __m128i out2 = _mm_or_si128(_mm_and_si128(mask_m0, m0_out2), _mm_andnot_si128(mask_m0, m1_out2));
    __m128i out3 = _mm_or_si128(_mm_and_si128(mask_m0, m0_out3), _mm_andnot_si128(mask_m0, m1_out3));

    return _mm_or_si128(_mm_or_si128(out0, _mm_slli_epi16(out1, 4)),
                        _mm_or_si128(_mm_slli_epi16(out2, 8), _mm_slli_epi16(out3, 12)));
}

// Unrolled PRINCE row shift
static inline uint64_t prince_shift_fast(uint64_t state) {
    return (state & 0xF000F000F000F000ULL) |
           (((state & 0x0F000F000F000F00ULL) << 16) | ((state & 0x0F000F000F000F00ULL) >> 48)) |
           (((state & 0x00F000F000F000F0ULL) << 32) | ((state & 0x00F000F000F000F0ULL) >> 32)) |
           (((state & 0x000F000F000F000FULL) << 48) | ((state & 0x000F000F000F000FULL) >> 16));
}

// Feistel mix
static inline void feistel_fast(uint64_t& w0, uint64_t& w1) {
    uint32_t d0 = (uint32_t)(w0 & 0xFFFFFFFF);
    uint32_t d1 = (uint32_t)((w0 >> 32) & 0xFFFFFFFF);
    uint32_t d2 = (uint32_t)(w1 & 0xFFFFFFFF);
    uint32_t d3 = (uint32_t)((w1 >> 32) & 0xFFFFFFFF);

    w0 = ((uint64_t)(d1 ^ d0) << 32) | d3;
    w1 = ((uint64_t)(d3 ^ d2) << 32) | d1;
}

// TKS shift
static inline void tks_shift_fast(uint64_t& w0, uint64_t& w1) {
    uint64_t new_w0 = (w0 & 0xF000F000F000F000ULL) |
                      ((w0 & 0x000000000F000F00ULL) << 32) | ((w1 & 0x0F000F0000000000ULL) >> 32) |
                      (w1 & 0x00F000F000F000F0ULL) |
                      ((w0 & 0x000F000F00000000ULL) >> 32) | ((w1 & 0x00000000000F000FULL) << 32);

    uint64_t new_w1 = (w1 & 0xF000F000F000F000ULL) |
                      ((w1 & 0x000000000F000F00ULL) << 32) | ((w0 & 0x0F000F0000000000ULL) >> 32) |
                      (w0 & 0x00F000F000F000F0ULL) |
                      ((w1 & 0x000F000F00000000ULL) >> 32) | ((w0 & 0x00000000000F000FULL) << 32);

    w0 = new_w0;
    w1 = new_w1;
}

// Pure SIMD TKS operating directly on __m128i register states
inline __m128i tweak_key_schedule(__m128i key128, __m128i tweak128, uint8_t R) {
    if (R == 0) return tweak128;

    __m128i sbox_lut = _mm_load_si128((const __m128i*)SBOX128);
    __m128i mask4 = _mm_set1_epi8(0x0F);

    __m128i state = tweak128;

    for (uint8_t r = 0; r < R; ++r) {
        __m128i rc = _mm_loadu_si128((const __m128i*)RC128[r]);
        state = _mm_xor_si128(state, _mm_xor_si128(key128, rc));
        state = sbox(state, sbox_lut, mask4);
        state = prince_m(state);

        alignas(16) uint64_t w[2];
        _mm_store_si128((__m128i*)w, state);

        w[1] = prince_shift_fast(w[1]);
        w[0] = prince_shift_fast(w[0]);

        feistel_fast(w[1], w[0]);
        tks_shift_fast(w[1], w[0]);

        state = _mm_load_si128((const __m128i*)w);
    }

    __m128i rc_final = _mm_loadu_si128((const __m128i*)RC128[R]);
    state = _mm_xor_si128(state, _mm_xor_si128(key128, rc_final));

    return state;
}

// Overloaded TKS for uint8_t[32] nibble arrays
inline void tweak_key_schedule(const uint8_t key[32], uint8_t tweak[32], uint8_t R) {
    alignas(16) uint64_t k_w[2] = {0, 0}, tw_w[2] = {0, 0};
    for (int i = 0; i < 16; i++) {
        k_w[1]  |= ((uint64_t)(key[i] & 0x0F) << (4 * (15 - i)));
        k_w[0]  |= ((uint64_t)(key[16 + i] & 0x0F) << (4 * (15 - i)));
        tw_w[1] |= ((uint64_t)(tweak[i] & 0x0F) << (4 * (15 - i)));
        tw_w[0] |= ((uint64_t)(tweak[16 + i] & 0x0F) << (4 * (15 - i)));
    }

    __m128i key128   = _mm_load_si128((const __m128i*)k_w);
    __m128i tweak128 = _mm_load_si128((const __m128i*)tw_w);

    __m128i res128 = tweak_key_schedule(key128, tweak128, R);

    alignas(16) uint64_t res_w[2];
    _mm_store_si128((__m128i*)res_w, res128);
    for (int i = 0; i < 16; i++) {
        tweak[i]      = (res_w[1] >> (4 * (15 - i))) & 0x0F;
        tweak[16 + i] = (res_w[0] >> (4 * (15 - i))) & 0x0F;
    }
}

#endif // TKS_H
