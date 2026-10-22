/*
 * A368229  Irregular table of nonnegative integers T(n, k), n >= 0, k =
 *          1..A001316(n), read by rows: the 1's in the binary expansion of n
 *          exactly match the nonzero digits in the ternary expansions of the
 *          terms in the n-th row.
 * https://oeis.org/A368229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   Constructed by rows of irregular table T(n,k) as described in
 *            OEIS entry A368229
 * Range:     u8 n <= 243, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Produces a(n) for OEIS sequence A368229. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_row = 0;

    static seq_t pow3_cache[64];
    static bool pow3_init = false;
    if (!pow3_init) {
        pow3_cache[0] = (seq_t)1;
        for (int i = 1; i < 64; ++i)
            pow3_cache[i] = pow3_cache[i-1] * (seq_t)3;
        pow3_init = true;
    }

    while (seq_len <= (size_t)n) {
        /* Positions of set bits in next_row */
        unsigned char pos[64];
        unsigned char w = 0;
        idx_t bits = next_row;
        unsigned int bit = 0;
        while (bits) {
            if (bits & 1)
                pos[w++] = bit;
            bits >>= 1;
            ++bit;
        }

        /* Base value: sum of 3^pos[i] */
        seq_t base = 0;
        for (unsigned char i = 0; i < w; ++i)
            base += pow3_cache[(idx_t)pos[i]];

        /* Row length = 2^w (or 1 when w==0) */
        idx_t row_len = 1;
        for (unsigned char i = 0; i < w; ++i)
            row_len <<= 1;

        /* Ensure capacity */
        size_t required = seq_len + (size_t)row_len;
        if (required > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 64;
            while (new_cap < required) {
                if (new_cap > SIZE_MAX / 2) {
                    new_cap = required;
                    break;
                }
                new_cap *= 2;
            }
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq = tmp;
            seq_cap = new_cap;
        }

        /* Generate the row */
        for (idx_t mask = 0; mask < row_len; ++mask) {
            seq_t val = base;
            for (unsigned char i = 0; i < w; ++i)
                if (mask & ((idx_t)1 << i))
                    val += pow3_cache[(idx_t)pos[i]];
            seq[seq_len++] = val;
        }

        ++next_row;
    }

    return seq[(size_t)n];
}
