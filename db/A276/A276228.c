/*
 * A276228  a(n+3) = -a(n+2) - 2*a(n+1) + a(n) with a(0)=3, a(1)=-1, a(2)=-3.
 * https://oeis.org/A276228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+3) = -a(n+2) - 2*a(n+1) + a(n) Initial values: a(0)=3,
 *            a(1)=-1, a(2)=-3
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 44, i64 n <= 92, i128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        size_t init_size = 3;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* fallback on allocation failure */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-3;
        max_cached = 2;
    }

    /* if requested index already cached, return it */
    if (n <= max_cached) return cache[n];

    /* extend cache up to index n */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;          /* fallback on allocation failure */
    cache = new_cache;

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = -cache[i - 1] - (seq_t)2 * cache[i - 2] + cache[i - 3];
    }
    max_cached = n;
    return cache[n];
}
