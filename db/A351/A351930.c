/*
 * A351930  Expansion of e.g.f. exp(x - x^4/24).
 * https://oeis.org/A351930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - binomial(n-1,3) * a(n-4) Base values: a(0) = 1,
 *            a(1) = 1, a(2) = 1, a(3) = 1
 * Range:     i8 n <= 8, i16 n <= 13, i32 n <= 18, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* Initialise cache with first four terms if not already done */
    if (cache == NULL) {
        cache_cap  = 4;
        cache_size = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) { /* OOM – let program abort in a useful way */
            return (seq_t)0;
        }
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
    }

    /* If requested index already computed, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Ensure capacity at least n+1 (simple doubling strategy) */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= n) {
            new_cap *= 2;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) { /* OOM */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* binomial(i-1,3) = (i-1)*(i-2)*(i-3)/6 */
        seq_t bin = (seq_t)(i - 1);
        bin *= (seq_t)(i - 2);
        bin *= (seq_t)(i - 3);
        bin /= (seq_t)6;

        cache[i] = cache[i - 1] - bin * cache[i - 4];
    }

    cache_size = n + 1;
    return cache[n];
}
