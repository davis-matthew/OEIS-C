/*
 * A375409  Expansion of e.g.f. exp(-x * (1 - x)) / (1 - x).
 * https://oeis.org/A375409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*(a(n-1)+3*a(n-2)-2*(n-2)*a(n-3)), with initial
 *            values a(0)=1, a(1)=0, a(2)=3, a(3)=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure cache can hold the value at index n */
static void ensure_cache(idx_t n) {
    /* No work needed if already cached */
    if ((size_t)n < cache_len) return;

    /* Initialise base cases on first call */
    if (cache_len == 0) {
        size_t min_cap = 4;
        if (cache_cap < min_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, min_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = min_cap;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)2;
        cache_len = 4;
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i < 4) continue;           /* base terms already set */
        seq_t term = (seq_t)(i - 1) *
            ( cache[i - 1]
              + (seq_t)3 * cache[i - 2]
              - (seq_t)2 * (seq_t)(i - 2) * cache[i - 3] );
        cache[i] = term;
    }
    cache_len = (size_t)n + 1;
}

/* Public function to obtain a(n) */
seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[(size_t)n];
}
