/*
 * A271893  Expansion of (2*x+1)*(1-x) / ( 1-2*x-4*x^2+6*x^3 ).
 * https://oeis.org/A271893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(0)=1, a(1)=3, a(2)=8,
 *            a(n)=2*a(n-1)+4*a(n-2)-6*a(n-3) for n≥3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    if (n >= cache_size) {
        /* expand cache */
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* fallback, unlikely to occur */

        /* initialise new entries if needed */
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        /* set initial values */
        if (new_cap > 0) new_cache[0] = (seq_t)1;
        if (new_cap > 1) new_cache[1] = (seq_t)3;
        if (new_cap > 2) new_cache[2] = (seq_t)8;
        cache   = new_cache;
        cache_cap   = new_cap;
        cache_size  = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = 3; (idx_t)i <= n; ++i) {
        cache[i] = (seq_t)(2) * cache[i-1]
                 + (seq_t)(4) * cache[i-2]
                 - (seq_t)(6) * cache[i-3];
    }

    return cache[n];
}
