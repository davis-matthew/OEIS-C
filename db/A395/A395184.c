/*
 * A395184  Totals of the base-3 run-length transform of the Jacobsthal
 *          numbers.
 * https://oeis.org/A395184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 3, a(2) = 9;
   for n ≥ 3, a(n) = 3*a(n-1) + 8*a(n-2) - 8*a(n-3). */
#include <stdlib.h>

/* Global cache for computed values */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

seq_t a(idx_t n)
{
    /* First allocation: ensure space for initially requested n */
    if (cache_len == 0) {
        cache_len = (n >= 3 ? n + 1 : 3);
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        /* Initialise the first three values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
    }

    /* If the requested index is already cached, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* Need to grow the cache to accommodate index n */
    idx_t old_len = cache_len;
    cache_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure: free old cache and return 0 */
        free(cache);
        cache = NULL;
        cache_len = 0;
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute new values only for indices beyond the previous length */
    idx_t start = old_len;
    if (start < 3)
        start = 3;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i - 1]
                 + (seq_t)8 * cache[i - 2]
                 - (seq_t)8 * cache[i - 3];
    }

    return cache[n];
}
