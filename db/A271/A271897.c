/*
 * A271897  Expansion of ( 1-2*x+3*x^2 ) / ( 1-4*x+5*x^2-4*x^3 ).
 * https://oeis.org/A271897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=6, a(n)=4*a(n-1)-5*a(n-2)+4*a(n-3) for n>=3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size of cache */
static idx_t  cached_n  = 0;   /* index of first not yet computed value */

/* ensure cache can hold at least 'needed' elements (needed>=1) */
static void ensure_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (cached_n == 0) {                /* first call – initialise base terms   */
        /* at least 3 slots are needed for a(0), a(1), a(2) */
        ensure_cache(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cached_n = 3;
    } else {
        /* enlarge cache if the requested index is beyond current capacity */
        if ((size_t)n + 1 > cache_cap)
            ensure_cache((size_t)n + 1);
    }

    if (n < cached_n)                  /* term already computed */
        return cache[n];

    /* compute missing terms up to n (inclusive) */
    for (idx_t i = cached_n; i <= n; ++i) {
        cache[i] = (4 * cache[i-1]) - (5 * cache[i-2]) + (4 * cache[i-3]);
    }
    cached_n = n + 1;                   /* next index that is not yet computed */
    return cache[n];
}
