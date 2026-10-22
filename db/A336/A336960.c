/*
 * A336960  E.g.f.: 1 / (1 - x * (2 + x) * exp(x)).
 * https://oeis.org/A336960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} binomial(n,k) * k * (k+1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for computed values. */
static seq_t *cache   = NULL;   /* array of a(i) */
static size_t cache_cap = 0;    /* allocated size of cache   */
static idx_t   computed  = 0;   /* largest n that has been filled in cache */

/* Resize the cache to accommodate indices up to n. */
static void ensure_cache(idx_t n) {
    if (cache == NULL) {
        /* First allocation. */
        size_t init = 4;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = init;
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[0] = 1;   /* a(0) = 1 */
        computed = 0;
        return;
    }
    /* Grow until capacity > n. */
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Compute a(n). */
seq_t a(idx_t n) {
    ensure_cache(n);
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t binom = 1; /* binomial(i,0) */
        for (idx_t k = 1; k <= i; ++k) {
            binom = binom * (seq_t)(i - k + 1);
            binom = binom / (seq_t)k;
            seq_t term = binom * (seq_t)k * (seq_t)(k + 1);
            term = term * cache[i - k];
            sum += term;
        }
        cache[i] = sum;
    }
    computed = n;
    return cache[n];
}
