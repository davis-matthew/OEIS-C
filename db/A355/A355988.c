/*
 * A355988  a(n) = n! / floor(n/3)!.
 * https://oeis.org/A355988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / floor(n/3)!
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function computes a(n) = n! / floor(n/3)!.
 * It memoises past results so that any order of calls is fast. */
seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure cache[0] is initialised */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;           /* out of memory in this context */
        cache[0] = (seq_t)1;
    }

    /* Return cached value if already computed */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    if (cache_size <= n) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;            /* out of memory */
        cache = tmp;
    }

    /* Compute missing values up to n */
    for (idx_t m = cache_size; m <= n; ++m) {
        if (m % 3 == 0) {
            /* a(m) = a(m-1) / (m/3) * m  (exact division) */
            seq_t q = (seq_t)(m / 3);
            seq_t temp = cache[m - 1] / q;
            cache[m] = temp * (seq_t)m;
        } else {
            /* a(m) = a(m-1) * m */
            cache[m] = cache[m - 1] * (seq_t)m;
        }
    }

    cache_size = n + 1;
    return cache[n];
}
