/*
 * A289321  a(n) = Sum_{k >= 0}(-1)^k*binomial(n,5*k+1).
 * https://oeis.org/A289321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k ≥ 0} (-1)^k * C(n, 5k+1) Recurrence: a(n) = 5*a(n-1)
 *            - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) for n ≥ 5 Initial values:
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=4
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 37, i64 n <= 69, i128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0..cache_size-1] holds computed values */
    static idx_t cache_size = 0;

    /* initialise cache for the five initial values */
    if (cache_size == 0) {
        cache_size = 5;                       /* indices 0..4 */
        cache = (seq_t *)realloc(NULL, cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 4;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* extend the cache to include index n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* compute missing values using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i-1]
                 - (seq_t)10 * cache[i-2]
                 + (seq_t)10 * cache[i-3]
                 - (seq_t)5 * cache[i-4];
    }

    cache_size = n + 1;
    return cache[n];
}
