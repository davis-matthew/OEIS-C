/*
 * A385121  a(n+1) = 12*a(n) - a(n-1), a(0) = a(1) = 2, a(n) = a(1-n).
 * https://oeis.org/A385121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = 12*a(n) - a(n-1), a(0) = a(1) = 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_last = (idx_t)-1; /* last computed index, -1 means empty */

    /* Lazy initialisation */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* fallback, should not happen */
        cache[0] = 2;
        cache[1] = 2;
        cache_last = 1;
    }

    /* Return from cache if already computed */
    if (n <= cache_last)
        return cache[n];

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        seq_t *newp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!newp)
            return 0; /* fallback, should not happen */
        cache = newp;
        cache_cap = n + 1;
    }

    /* Compute new terms up to n */
    seq_t a_nm1 = cache[cache_last >= 1 ? cache_last - 1 : 0];
    seq_t a_n   = cache[cache_last];

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t a_np1 = (seq_t)12 * a_n - a_nm1;
        cache[i] = a_np1;
        a_nm1 = a_n;
        a_n = a_np1;
    }

    cache_last = n;
    return cache[n];
}
