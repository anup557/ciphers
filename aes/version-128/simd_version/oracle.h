// Helper macro to process the output of _mm_aeskeygenassist_si128
// It performs the sliding XORs across the 32-bit words of the key
#define AES_KEY_EXPAND_128(k_curr, k_assist) { \
    k_assist = _mm_shuffle_epi32(k_assist, _MM_SHUFFLE(3, 3, 3, 3)); \
    k_curr   = _mm_xor_si128(k_curr, _mm_slli_si128(k_curr, 4));      \
    k_curr   = _mm_xor_si128(k_curr, _mm_slli_si128(k_curr, 4));      \
    k_curr   = _mm_xor_si128(k_curr, _mm_slli_si128(k_curr, 4));      \
    k_curr   = _mm_xor_si128(k_curr, k_assist);                       \
}

void key_schedule(__m128i key, __m128i *rnd_keys) {
    // Round 0 key is just the original master key
    rnd_keys[0] = key;

    __m128i temp = rnd_keys[0];
    __m128i assist;

    // Round 1 (Rcon = 0x01)
    assist = _mm_aeskeygenassist_si128(temp, 0x01);
    AES_KEY_EXPAND_128(temp, assist);
    rnd_keys[1] = temp;

    // Round 2 (Rcon = 0x02)
    assist = _mm_aeskeygenassist_si128(temp, 0x02);
    AES_KEY_EXPAND_128(temp, assist);
    rnd_keys[2] = temp;

    // Round 3 (Rcon = 0x04)
    assist = _mm_aeskeygenassist_si128(temp, 0x04);
    AES_KEY_EXPAND_128(temp, assist);
    rnd_keys[3] = temp;

    // Round 4 (Rcon = 0x08)
    assist = _mm_aeskeygenassist_si128(temp, 0x08);
    AES_KEY_EXPAND_128(temp, assist);
    rnd_keys[4] = temp;

    // Round 5 (Rcon = 0x10)
    assist = _mm_aeskeygenassist_si128(temp, 0x10);
    AES_KEY_EXPAND_128(temp, assist);
    rnd_keys[5] = temp;

    // Round 6 (Rcon = 0x20)
    assist = _mm_aeskeygenassist_si128(temp, 0x20);
    AES_KEY_EXPAND_128(temp, assist);
    rnd_keys[6] = temp;

    // Round 7 (Rcon = 0x40)
    assist = _mm_aeskeygenassist_si128(temp, 0x40);
    AES_KEY_EXPAND_128(temp, assist);
    rnd_keys[7] = temp;

    // Round 8 (Rcon = 0x80)
    assist = _mm_aeskeygenassist_si128(temp, 0x80);
    AES_KEY_EXPAND_128(temp, assist);
    rnd_keys[8] = temp;

    // Round 9 (Rcon = 0x1B)
    assist = _mm_aeskeygenassist_si128(temp, 0x1B);
    AES_KEY_EXPAND_128(temp, assist);
    rnd_keys[9] = temp;

    // Round 10 (Rcon = 0x36)
    assist = _mm_aeskeygenassist_si128(temp, 0x36);
    AES_KEY_EXPAND_128(temp, assist);
    rnd_keys[10] = temp;
}


__m128i enc(__m128i state, __m128i *rnd_key) {
    // Round 0: Initial AddRoundKey
    state = _mm_xor_si128(state, rnd_key[0]);

    // Rounds 1 to 9: Full AES Rounds
    state = _mm_aesenc_si128(state, rnd_key[1]);
    state = _mm_aesenc_si128(state, rnd_key[2]);
    state = _mm_aesenc_si128(state, rnd_key[3]);
    state = _mm_aesenc_si128(state, rnd_key[4]);
    state = _mm_aesenc_si128(state, rnd_key[5]);
    state = _mm_aesenc_si128(state, rnd_key[6]);
    state = _mm_aesenc_si128(state, rnd_key[7]);
    state = _mm_aesenc_si128(state, rnd_key[8]);
    state = _mm_aesenc_si128(state, rnd_key[9]);

    // Round 10: Final AES Round
    state = _mm_aesenclast_si128(state, rnd_key[10]);

    return state;
}


