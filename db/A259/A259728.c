/*
 * A259728  Sum of digits of a(n) equals the sum of digits of 4*a(n).
 * https://oeis.org/A259728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n‑th term of OEIS sequence A259728:
   numbers a whose digit sum equals the digit sum of 4*a.
   The sequence consists of all multiples of 3 that satisfy
   digit_sum(a) == digit_sum(4*a). */

#include <stdlib.h>
#include "../utils/digit_sum.h"

/* Return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_mult = 3;   /* next multiple of 3 to test */

    /* Ensure the cache contains the first term (0). */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache_len = 1;
        next_mult = 3;
    }

    /* Grow cache while needed. */
    while (cache_len < n) {
        seq_t cand = next_mult;
        if (digit_sum(cand) == digit_sum(cand * (seq_t)4)) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }
        next_mult += (seq_t)3;
    }

    return cache[n - 1];
}
