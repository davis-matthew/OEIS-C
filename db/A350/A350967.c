/*
 * A350967  a(n) = sqrt(84*A144930(n)^2 - 3).
 * https://oeis.org/A350967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 110*a(n-1) - a(n-2), a(1)=9, a(2)=999
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* This recurrence comes from the generating function 9*x*(1+x)/(1-110*x+x^2). */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
    static idx_t cached_len = 0;     /* number of elements stored */

    /* initialise cache with first two terms */
    if (cached_len == 0) {
        cached_len = 2;
        cache = (seq_t*)malloc(cached_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)9;              /* a(1) */
        cache[1] = (seq_t)999;            /* a(2) */
    }

    /* If the requested term is already cached, return it. */
    if (n <= cached_len)
        return cache[n - 1];

    /* Extend the cache up to the requested index n. */
    seq_t *new_cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;      /* allocation failure */
    cache = new_cache;

    for (idx_t i = cached_len; i < n; ++i) {
        /* recurrence: a(i+1) = 110*a(i) - a(i-1)  (i>=2) */
        cache[i] = (seq_t)110 * cache[i - 1] - cache[i - 2];
    }
    cached_len = n;

    return cache[n - 1];
}
