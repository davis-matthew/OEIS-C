/*
 * A321087  O.g.f. A(x) satisfies: [x^n] exp(n*A(x)) * (1 - n*x/(1-x)) = 0,
 *          for n > 0.
 * https://oeis.org/A321087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum_{i=1}^{n-1} a(i) * (n-i) * a(n-i) * (with a(1) =
 *            1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required for memory allocation and size types */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* Array of computed terms, index 0 unused */
static idx_t  cached_max = 0;    /* Largest index computed so far */
static size_t cache_cap   = 0;    /* Allocated capacity */

/* Grow the cache array to at least new_cap elements */
static void cache_grow_to(size_t new_cap) {
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap : 128;
    while (cap < new_cap) cap <<= 1;          /* grow geometrically */
    seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * cap);
    for (size_t i = 0; i < cap; ++i) new_cache[i] = 0;
    if (cache) {
        for (size_t i = 0; i <= cached_max; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache      = new_cache;
    cache_cap  = cap;
}

/* Ensure that cache has computed terms up to index n (inclusive) */
static void ensure_cache(idx_t n) {
    if (n <= cached_max) return;
    cache_grow_to((size_t)n + 1);
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = 1;                /* a(1) = 1 */
        } else {
            seq_t sum = 0;
            for (idx_t i = 1; i < k; ++i) {
                /* sum += a(i) * (k-i) * a(k-i) */
                sum += cache[i] * cache[k - i] * (seq_t)(k - i);
            }
            cache[k] = sum + 1;           /* a(k) = 1 + sum */
        }
    }
    cached_max = n;
}

/* Public function required by the platform */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset not used */
    ensure_cache(n);
    return cache[n];
}
