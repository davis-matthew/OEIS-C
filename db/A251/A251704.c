/*
 * A251704  4-step Fibonacci sequence starting with 1, 1, 0, 1.
 * https://oeis.org/A251704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = a(n) + a(n+1) + a(n+2) + a(n+3) with initial values
 *            a(0)=1, a(1)=1, a(2)=0, a(3)=1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed values */
static seq_t *cache = NULL;          /* allocated cache array */
static size_t cache_cap = 0;         /* current capacity of the array */
static idx_t cache_len = 0;          /* number of values already computed (indices 0 .. cache_len-1) */

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Allocate initial storage if needed */
    if (cache == NULL) {
        /* Start with a modest capacity, at least enough for the requested index */
        cache_cap = n + 1;
        if (cache_cap < 16) cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 0;
    }

    /* Grow the cache if the requested index is beyond the current capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)        cache[i] = (seq_t)1;
        else if (i == 1)   cache[i] = (seq_t)1;
        else if (i == 2)   cache[i] = (seq_t)0;
        else if (i == 3)   cache[i] = (seq_t)1;
        else               cache[i] = cache[i-4] + cache[i-3] + cache[i-2] + cache[i-1];
    }
    cache_len = n + 1;
}

/* Public function that returns the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n >= cache_len) ensure_cache(n);
    return cache[n];
}
