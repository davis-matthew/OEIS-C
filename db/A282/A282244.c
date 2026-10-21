/*
 * A282244  Lexicographic block-fractal zero-one word with initial block 01.
 * https://oeis.org/A282244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth bit (1‑based) of the lexicographic block‑fractal
 *            binary word with initial block 01. The word is built by, for k =
 *            2,3,…, appending all binary strings of length k that do not yet
 *            occur as a substring, in lexicographic order
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state */
static uint8_t *bit_buf = NULL;   /* holds 0/1 bits */
static size_t   bit_len = 0;      /* number of bits stored */
static size_t   bit_cap = 0;      /* allocated capacity */
static size_t   cur_k   = 2;      /* current block length being processed */

/* ensure that the bit buffer can hold 'need' bits */
static void ensure_cap(size_t need)
{
    if (bit_cap >= need)
        return;
    size_t new_cap = bit_cap ? bit_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(bit_buf, new_cap);
    bit_buf = tmp;
    bit_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1 – not relevant for valid n */
        return (seq_t)0;

    /* initialise with the seed word "01" */
    if (bit_len == 0) {
        ensure_cap(2);
        bit_buf[0] = 0;
        bit_buf[1] = 1;
        bit_len = 2;
    }

    /* grow the word until it has at least n bits */
    while (bit_len < (size_t)n) {
        /* number of possible k‑bit strings */
        size_t mask_sz = (size_t)1 << cur_k;
        /* mark which k‑bit strings already appear */
        uint8_t *present = (uint8_t *)calloc(mask_sz, 1);
        if (bit_len >= cur_k) {
            uint64_t val = 0;
            for (size_t i = 0; i < cur_k; ++i)
                val = (val << 1) | bit_buf[i];
            present[val] = 1;
            uint64_t mask = ((uint64_t)1 << cur_k) - 1;
            for (size_t i = 1; i + cur_k <= bit_len; ++i) {
                val = ((val << 1) & mask) | bit_buf[i + cur_k - 1];
                present[val] = 1;
            }
        }
        /* append all missing k‑bit strings in lexicographic order */
        for (size_t j = 0; j < mask_sz; ++j) {
            if (!present[j]) {
                for (int b = (int)cur_k - 1; b >= 0; --b) {
                    ensure_cap(bit_len + 1);
                    bit_buf[bit_len++] = (uint8_t)((j >> b) & 1);
                }
            }
        }
        free(present);
        ++cur_k;
    }

    return (seq_t)bit_buf[(size_t)n - 1];
}
