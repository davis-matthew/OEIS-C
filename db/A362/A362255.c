/*
 * A362255  a(0) = a(1) = a(2) = 1, for n > 2, a(n) = a(n-1) + a(n-k) + k with
 *          k = 2.
 * https://oeis.org/A362255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=1, a(3)=4, a(n) = 2*a(n-1) - a(n-2) + a(n-3) -
 *            a(n-4) for n > 3
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>


seq_t a(idx_t n)
{
    /* Local cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_alloc = 0;   /* allocated array size   */
    static idx_t cache_last  = (idx_t)3; /* highest index computed */

    /* Base cases */
    if (n <= 2)
        return (seq_t)1;
    if (n == 3)
    {
        if (cache == NULL)  /* first call, initialise cache */
        {
            cache_alloc = 4;
            cache = (seq_t *)calloc(cache_alloc, sizeof(seq_t));
            if (!cache) return (seq_t)0;   /* allocation failed */
            cache[0] = cache[1] = cache[2] = 1;
            cache[3] = 4;
        }
        return cache[3];
    }

    /* Initialise cache on first call (when cache is not yet allocated) */
    if (cache == NULL)
    {
        cache_alloc = 4;
        cache = (seq_t *)calloc(cache_alloc, sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = cache[1] = cache[2] = 1;
        cache[3] = 4;
    }

    /* Expand the cache if necessary */
    if (cache_alloc <= (size_t)n)
    {
        size_t new_alloc = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* Compute any missing values up to n */
    if (cache_last < n)
    {
        for (idx_t i = cache_last + 1; i <= n; ++i)
        {
            cache[i] = ((seq_t)2 * cache[i-1])
                       - cache[i-2]
                       + cache[i-3]
                       - cache[i-4];
        }
        cache_last = n;
    }

    return cache[n];
}
