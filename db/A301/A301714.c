/*
 * A301714  Coordination sequence for node of type V2 in "usm" 2-D tiling (or
 *          net).
 * https://oeis.org/A301714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) - a(n-4) + 2*a(n-5) - a(n-6) + a(n-9) - a(n-10)   for n >= 11
 * a(0) = 1, a(1) = 5, a(2) = 12, a(3) = 16, a(4) = 21, a(5) = 28,
 * a(6) = 31, a(7) = 38, a(8) = 44, a(9) = 47
 * a(10) = 1 + a(9) - a(6) + 2*a(5) - a(4) + a(1) - a(0)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;   /* number of computed terms */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Initial 10 terms (a(0) .. a(9)) */
    const seq_t init_vals[10] = {
        (seq_t)1, (seq_t)5, (seq_t)12, (seq_t)16, (seq_t)21,
        (seq_t)28, (seq_t)31, (seq_t)38, (seq_t)44, (seq_t)47
    };

    /* if the requested index is within the initial block */
    if (n < 10)
        return init_vals[n];

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        if (cache_cap < 64) cache_cap = 64;          /* small grow buffer */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                  /* allocation protection */

        /* store the first 10 values */
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init_vals[i];
        cache_len = 10;
    }

    /* grow the cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                   /* allocation protection */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 10) {
            /* a(10) = 1 + a(9) - a(6) + 2*a(5) - a(4) + a(1) - a(0) */
            seq_t pos = cache[9] + cache[5] + cache[5] + cache[1] + (seq_t)1;
            seq_t neg = cache[6] + cache[4] + cache[0];
            cache[10] = pos - neg;
        } else { /* i >= 11 */
            /* a(i) = a(i-1) - a(i-4) + 2*a(i-5) - a(i-6) + a(i-9) - a(i-10) */
            seq_t pos = cache[i-1] + cache[i-5] + cache[i-5] + cache[i-9];
            seq_t neg = cache[i-4] + cache[i-6] + cache[i-10];
            cache[i] = pos - neg;
        }
    }
    cache_len = n + 1;

    return cache[n];
}
