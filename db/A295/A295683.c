/*
 * A295683  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 2, a(1) = 1, a(2) =
 *          0, a(3) = 1.
 * https://oeis.org/A295683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), a(0) = 2, a(1) = 1, a(2) = 0,
 *            a(3) = 1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* Static cache to store computed values and avoid recomputation. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* Number of valid entries in cache. */
static size_t cache_cap = 0;   /* Allocated capacity of cache. */

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Initial allocation with first four terms. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        assert(cache != NULL);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache_size = 4;
    }

    /* Grow the cache if requested index exceeds current capacity. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(new_cache != NULL);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute new values iteratively using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* For i >= 4, all required indices are valid. */
        cache[(size_t)i] = cache[(size_t)(i - 1)] +
                           cache[(size_t)(i - 3)] +
                           cache[(size_t)(i - 4)];
    }

    cache_size = (idx_t)(cache_cap); /* All up to cache_cap-1 are valid. */
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
