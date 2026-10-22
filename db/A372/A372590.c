/*
 * A372590  Numbers whose binary weight (A000120) plus bigomega (A001222) is
 *          odd.
 * https://oeis.org/A372590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers whose binary weight (A000120) plus bigomega (A001222) is
 *            odd
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bigomega.h"

/* Count the number of 1-bits in an idx_t value.  idx_t is unsigned 64‑bit. */
static inline idx_t popcount_idx(idx_t x)
{
    return __builtin_popcountll(x);
}

/* Sequence A372590 – numbers k with popcount(k)+bigomega(k) odd. */
/* Memoizes values in a static array so calls in any order are fast. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t next_int = 0;   /* first integer not yet tested (exclusive) */

    while (cache_len < n) {
        /* Test the next integer */
        ++next_int;
        idx_t w = popcount_idx(next_int);
        seq_t bo = bigomega(next_int);

        /* The sum is odd iff the bits differ modulo 2 */
        if (((w & 1) ^ ((idx_t)(bo & 1))) != 0) {
            /* Make room if necessary */
            if (cache_cap == 0) {
                cache_cap = 128;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            } else if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_int;
        }
    }
    return cache[n - 1];
}
