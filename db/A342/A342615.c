/*
 * A342615  a(0) = 0, a(1) = 1; a(2*n) = 9*a(n), a(2*n+1) = a(n) + a(n+1).
 * https://oeis.org/A342615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=0, a(1)=1; a(2*n)=9*a(n), a(2*n+1)=a(n)+a(n+1)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static size_t cache_cap = 0;

/* Ensure that the cache contains values for all indices up to n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate initial space for indices 0 and 1. */
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort(); /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing entries. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if ((i & 1) == 0) {
            cache[i] = (seq_t)9 * cache[i >> 1];
        } else {
            cache[i] = cache[(i - 1) >> 1] + cache[(i + 1) >> 1];
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
