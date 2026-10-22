/*
 * A366942  Expansion of e.g.f. 1/(1-x-2*x^2-3*x^3).
 * https://oeis.org/A366942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(a(n-1)+(n-1)*(2*a(n-2)+(n-2)*3*a(n-3))), with a(0)=1,
 *            a(1)=1, a(2)=6. The function memoizes results so it works for
 *            arbitrary order of calls
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
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
    static idx_t   cache_len = 0;  /* highest computed index + 1 */
    static size_t  cache_cap = 0;  /* allocated capacity */

    /* If already computed, just return */
    if (n < cache_len) {
        return cache[n];
    }

    /* Ensure cache large enough for at least indices 0..n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 32;
        while (new_cap < (size_t)(n + 1)) {
            new_cap <<= 1;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) {
            /* Out of memory; abort */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise base cases if this is the first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache_len = 3;
    }

    /* Compute missing terms up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* For i=0,1,2 recursion not used because they are already set */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];

        /* term = a(i-1) + (i-1)*(2*a(i-2) + (i-2)*3*a(i-3)) */
        seq_t term = a1
                   + ((seq_t)(i - 1)
                      * (((seq_t)2 * a2)
                         + ((seq_t)(i - 2) * (seq_t)3 * a3)));

        cache[i] = (seq_t)i * term;
    }

    cache_len = n + 1;
    return cache[n];
}
