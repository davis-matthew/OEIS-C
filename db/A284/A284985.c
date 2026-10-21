/*
 * A284985  a(0)=0, a(1)=24; for n>=2, a(n)=576*a(n-1)-a(n-2).
 * https://oeis.org/A284985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 576*a(n-1) - a(n-2), a(0) = 0, a(1) = 24
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A284985 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoization array */
    static size_t cache_cap = 0;     /* current capacity of cache   */
    static size_t cache_last = 0;    /* index of last computed term */

    /* initialise cache for the first two terms */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();              /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)24;
        cache_last = 1;
    }

    /* if the requested term is already known, return it */
    if (n <= cache_last) {
        return cache[(size_t)n];
    }

    /* enlarge the cache to accommodate index n (double strategy) */
    size_t old_cap = cache_cap;
    while (cache_cap <= (size_t)n) {
        cache_cap <<= 1;                 /* double until large enough */
    }
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute missing terms iteratively */
    for (size_t i = cache_last + 1; i <= (size_t)n; ++i) {
        cache[i] = ((seq_t)576 * cache[i - 1]) - cache[i - 2];
    }
    cache_last = (size_t)n;

    return cache[n];
}
