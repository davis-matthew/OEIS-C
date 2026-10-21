/*
 * A295734  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 0,
 *          a(1) = 0, a(2) = -1, a(3) = 2.
 * https://oeis.org/A295734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) initial values:
 *            a(0) = 0, a(1) = 0, a(2) = -1, a(3) = 2
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 44, i64 n <= 90, i128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    switch (n)
    {
        case 0: return 0;
        case 1: return 0;
        case 2: return (seq_t)-1;
        case 3: return 2;
    }

    /* Static cache to allow repeated calls in arbitrary order. */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cache_size = 0; /* number of filled elements */

    /* Grow the cache if necessary. */
    if (cache_size <= (size_t)n)
    {
        /* New size needed is n+1. */
        size_t new_size = (size_t)n + 1;
        if (cache == NULL)
        {
            /* First allocation. */
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            assert(cache != NULL);
            /* Set the initial four values. */
            cache[0] = 0;
            cache[1] = 0;
            cache[2] = (seq_t)-1;
            cache[3] = 2;
            cache_size = new_size;
            /* Compute all terms up to n. */
            for (idx_t i = 4; i <= n; ++i)
                cache[i] = cache[i - 1] + 3 * cache[i - 2]
                           - 2 * cache[i - 3] - 2 * cache[i - 4];
        }
        else
        {
            /* Extend the existing cache. */
            seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            assert(new_cache != NULL);
            cache = new_cache;
            /* Compute the newly‑needed terms. */
            for (idx_t i = (idx_t)cache_size; i <= n; ++i)
                cache[i] = cache[i - 1] + 3 * cache[i - 2]
                           - 2 * cache[i - 3] - 2 * cache[i - 4];
            cache_size = new_size;
        }
    }

    return cache[n];
}
