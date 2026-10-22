/*
 * A351531  a(0)=1; a(1)=1; for n>1, a(n) = a(n-1) + 3*n*a(n-2).
 * https://oeis.org/A351531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 1; for n > 1, a(n) = a(n-1) + 3*n*a(n-2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache for memoised values */
static seq_t *cache = NULL;      /* array of sequence values */
static idx_t  cache_len = 0;     /* number of elements in cache */

/* Ensure the cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    /* No cache yet – allocate minimal required space */
    if (cache == NULL)
    {
        /* Always allocate at least two slots to store a(0) and a(1) */
        size_t alloc_len = (n >= 1 ? (size_t)n + 1 : 2);
        cache = (seq_t*)malloc(alloc_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        if (alloc_len > 1)
            cache[1] = (seq_t)1;
        cache_len = alloc_len;

        /* Compute missing terms if needed */
        for (idx_t i = (idx_t)2; i <= n; ++i)
        {
            cache[i] = cache[i-1] + (seq_t)3 * (seq_t)i * cache[i-2];
        }
        return;
    }

    /* Cache already exists – if large enough, nothing to do */
    if (cache_len > n)
        return;

    /* Grow the cache to hold up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
    memcpy(new_cache, cache, cache_len * sizeof(seq_t));

    /* Compute new terms */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        new_cache[i] = new_cache[i-1] + (seq_t)3 * (seq_t)i * new_cache[i-2];
    }

    free(cache);
    cache = new_cache;
    cache_len = new_len;
}

/* Public function required by the build environment */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
