/*
 * A309791  Expansion of (1 + 8*x - 6*x^2 + 12*x^3 - 18*x^4)/(1 - x - 9*x^4 +
 *          9*x^5).
 * https://oeis.org/A309791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 9*a(n-4) - 9*a(n-5) for n>=5, initial values:
 *            a(0)=1, a(1)=9, a(2)=3, a(3)=15, a(4)=6
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache: buffers the computed sequence values up to the
       highest requested index.  The cache is grown as needed. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of values actually cached */
    static size_t cache_cap  = 0;   /* allocated capacity of 'cache' */

    /* Initial terms for indices 0..4 */
    static const seq_t init[5] = { 1, 9, 3, 15, 6 };

    /* Return directly if within the initial window. */
    if (n < 5)
        return init[n];

    /* Grow the cache to at least (n+1) elements. */
    while (cache_size <= n) {
        /* Allocate / reallocate if necessary. */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            /* Ensure we have space for at least cache_size + 1 elements. */
            while (new_cap <= cache_size)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;  /* out of memory: return 0 as sentinel */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute the next term using the linear recurrence. */
        seq_t term;
        if (cache_size < 5) {
            /* Should not happen for cache_size >=5 needed in loop,
               but keep for consistency. */
            term = init[cache_size];
        } else {
            term = cache[cache_size - 1]
                 + (seq_t)9 * cache[cache_size - 4]
                 - (seq_t)9 * cache[cache_size - 5];
        }

        cache[cache_size++] = term;
    }

    return cache[n];
}
