/*
 * A356569  Sums of powers of roots of x^4 - 2*x^3 - 6*x^2 + 2*x + 1.
 * https://oeis.org/A356569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 2*a(n-1) + 6*a(n-2) - 2*a(n-3) - a(n-4), * a(0)=4,
 *            a(1)=2, a(2)=16, a(3)=38
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* current cache size (terms 0 .. cache_size-1) */

    /* Initialise the cache with the first four terms the first time we are called */
    if (!cache) {
        cache_size = 4;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory guard */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)38;
    }

    /* If the requested index is already in the cache, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* Expand the cache to at least n+1 elements */
    size_t new_size = (size_t)n + 1;
    if (new_size < 4) new_size = 4;           /* ensure space for the base terms */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;               /* out‑of‑memory guard */
    cache = tmp;
    cache_size = (idx_t)new_size;

    /* Compute any new terms up to index n */
    size_t start = (size_t)cache_size - 1;    /* first index already computed */
    if (start < 4) start = 4;                 /* start from the first term that
                                               needs the recurrence */
    for (size_t i = start; i <= (size_t)n; ++i) {
        /* a(i) = 2*a(i-1) + 6*a(i-2) - 2*a(i-3) - a(i-4) */
        cache[i] = cache[i-1] * (seq_t)2 
                 + cache[i-2] * (seq_t)6
                 - cache[i-3] * (seq_t)2
                 - cache[i-4];
    }

    return cache[n];
}
