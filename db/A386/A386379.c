/*
 * A386379  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/5)} a(5*k) * a(n-1-5*k).
 * https://oeis.org/A386379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=0 a(n) = Σ_{k=0..⌊(n-1)/5⌋} a(5k) * a(n-1-5k) for
 *            n≥1
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A386379, memoized across calls. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of computed entries */

    /* Ensure we have computed a[0] at least */
    if (cache_len == 0) {
        if (cache_cap < 1) {
            cache_cap = 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) abort();
        }
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* if n already computed, return it */
    if ((size_t)n < cache_len) return cache[n];

    /* Grow cache to accommodate up to n */
    if (cache_cap < (size_t)(n+1)) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        if (new_cap < (size_t)(n+1)) new_cap = (size_t)(n+1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values from cache_len up to n */
    for (size_t m = cache_len; m <= (size_t)n; ++m) {
        seq_t sum = (seq_t)0;
        if (m == 0) {          /* already set, skip */
            cache[m] = (seq_t)1;
            continue;
        }
        /* floor((m-1)/5) */
        idx_t maxk = (idx_t)((m - 1) / 5);
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t idx1 = 5 * k;
            idx_t idx2 = (idx_t)(m - 1) - 5 * k;
            sum += cache[idx1] * cache[idx2];
        }
        cache[m] = sum;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
