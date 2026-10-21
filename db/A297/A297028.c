/*
 * A297028  Number of edge covers in the 2n-crossed prism graph.
 * https://oeis.org/A297028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*a(n-1) - 75*a(n-2) + 12*a(n-3) Initial values:
 *            a(1)=43, a(2)=1699, a(3)=69868
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
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
    static idx_t cache_cap = 0;   /* allocated array size */
    static idx_t cache_max = 0;   /* greatest index that has a computed value */

    if (n < 1)               /* safety for offset */
        return (seq_t)0;

    /* ensure capacity to store indices up to n (1‑based) */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap : 4;   /* at least indices 1..3 */
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base cases if not yet present */
    if (cache_max < 3) {
        if (cache_max < 1)
            cache[1] = (seq_t)43;
        if (cache_max < 2)
            cache[2] = (seq_t)1699;
        if (cache_max < 3)
            cache[3] = (seq_t)69868;
        if (cache_max < 3)
            cache_max = 3;
    }

    /* if requested value already computed, return it */
    if (n <= cache_max)
        return cache[n];

    /* compute missing values using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* recurrence only valid for i >= 4, but loop starts from >cache_max */
        seq_t val = (seq_t)43 * cache[i - 1]
                    - (seq_t)75 * cache[i - 2]
                    + (seq_t)12 * cache[i - 3];
        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
