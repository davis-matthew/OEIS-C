/*
 * A241718  The number of P-positions in the game of Nim with up to 4 piles,
 *          allowing for piles of zero, such that the number of objects in the
 *          largest pile is n.
 * https://oeis.org/A241718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*c-6)*2^b + a(c-1) where b = ⌊log₂(n)⌋, c = n + 1 -
 *            2^b, a(0) = 1 (OEIS A241718)
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* static cache: cache[i] contains a(i) for 0 ≤ i < cap */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Extend cache so that indices 0..n are present */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (cache == NULL) abort(); /* allocation failed */
        } else {
            seq_t *tmp = (seq_t *)realloc(cache,
                                            new_cap * sizeof(seq_t));
            if (tmp == NULL) abort(); /* reallocation failed */
            cache = tmp;
        }

        /* Fill missing entries from current cap up to new_cap-1 */
        for (size_t idx = cap; idx < new_cap; ++idx) {
            if (idx == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            idx_t i = (idx_t)idx;
            unsigned b = floor_log2_uint64(i);          /* b = ⌊log₂ i⌋ */
            seq_t pow2b = pow2((idx_t)b);               /* 2^b          */
            seq_t c = (seq_t)i + (seq_t)1 - pow2b;      /* c = i+1-2^b   */
            seq_t a_c_minus1 = cache[(size_t)(c - (seq_t)1)];
            seq_t temp = (seq_t)12 * c - (seq_t)6;      /* 12·c−6        */
            cache[idx] = temp * pow2b + a_c_minus1;     /* formula       */
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
