/*
 * A335720  a(n) = 2*a(n-1) + 3*a(n-2) + 5*a(n-3), a(0) = 0, a(1) = 1, a(2) =
 *          1.
 * https://oeis.org/A335720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 5*a(n-3); a(0) = 0, a(1) = 1, a(2)
 *            = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values; shared across calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of entries currently stored */
    static size_t cache_cap = 0; /* allocated capacity */

    /* initialize cache for the first call */
    if (cache == NULL) {
        cache_cap = 3;                        /* indices 0..2 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache_size = 3;
    }

    /* return immediately if already computed */
    if (n < cache_size) {
        return cache[n];
    }

    /* ensure enough capacity for indices up to n */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) {
            new_cap *= 2;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms in order */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 0;
        } else if (i == 1) {
            cache[1] = 1;
        } else if (i == 2) {
            cache[2] = 1;
        } else {
            cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2] + 5 * cache[i - 3];
        }
    }

    cache_size = n + 1;   /* now indices 0..n are valid */
    return cache[n];
}
