/*
 * A249060  Column 1 of the triangular array at A249057.
 * https://oeis.org/A249060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)*a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence of order 2; simple iterative computation. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to memoize results across calls. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure cache has at least the first two terms. */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* Out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
    }

    /* Extend cache up to index n if necessary. */
    if ((idx_t)cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;      /* Out of memory */
        cache = new_cache;

        /* Compute missing terms iteratively from cache_size to n. */
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = (seq_t)(i + 3) * cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
