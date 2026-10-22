/*
 * A351586  Number of minimal edge covers of the 2n-crossed prism graph.
 * https://oeis.org/A351586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 12*a(n-2) + 40*a(n-3) + 56*a(n-4) - 8*a(n-5) -
 *            32*a(n-6) for n > 6 Initial values: a(1)=5, a(2)=49, a(3)=425,
 *            a(4)=3137, a(5)=22985, a(6)=172081
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* base values for n = 1..6 */
    static const seq_t BASE[6] = {
        (seq_t)5,
        (seq_t)49,
        (seq_t)425,
        (seq_t)3137,
        (seq_t)22985,
        (seq_t)172081
    };

    /* if cached, return */
    if ((size_t)n <= cache_size) {
        return cache[(size_t)n - 1];
    }

    /* enlarge cache to n elements (for 1-based indexing) */
    size_t new_size = (size_t)n;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    /* if realloc failed, new cache will be NULL; but caller will handle. */

    /* compute new values */
    for (size_t i = cache_size; i < new_size; ++i) {
        idx_t idx = (idx_t)i + 1; /* 1-based index */
        if (idx <= 6) {
            cache[i] = BASE[idx - 1];
        } else {
            /* recurrence using unsigned arithmetic */
            cache[i] = 5 * cache[i - 1]      /* a(n-1) */
                    + 12 * cache[i - 2]      /* a(n-2) */
                    + 40 * cache[i - 3]      /* a(n-3) */
                    + 56 * cache[i - 4]      /* a(n-4) */
                    -  8 * cache[i - 5]      /* a(n-5) */
                    - 32 * cache[i - 6];     /* a(n-6) */
        }
    }

    cache_size = new_size;
    return cache[(size_t)n - 1];
}
