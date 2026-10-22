/*
 * A305133  E.g.f.: (1-x) / (exp(-x) - x).
 * https://oeis.org/A305133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n * Σ_{k=1}^{n-1} C(n‑1, k) * a(k) (a(0) = 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache if first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    if (n < cache_size) /* already computed */
        return cache[n];

    /* enlarge cache to hold indices up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;

        for (idx_t k = 1; k < i; ++k) {
            /* binom = binom * (i - k) / k   (C(i-1,k)) */
            binom = binom * (seq_t)(i - k) / (seq_t)k;
            sum += binom * cache[k];
        }
        cache[i] = (seq_t)1 + (seq_t)i * sum;
    }

    cache_size = n + 1;
    return cache[n];
}
