/*
 * A255280  A255279(2^n-1).
 * https://oeis.org/A255280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 9*a(n-3) + 16*a(n-4) - 16*a(n-6) -
 *            16*a(n-7), n ≥ 7 Initial values: a(0)=1, a(1)=7, a(2)=25,
 *            a(3)=107, a(4)=413, a(5)=1615, a(6)=6241
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial seven terms */
    static const seq_t init[7] = {
        (seq_t)1, (seq_t)7, (seq_t)25, (seq_t)107,
        (seq_t)413, (seq_t)1615, (seq_t)6241
    };

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of terms currently computed */

    /* Allocate cache on first call */
    if (cache == NULL) {
        cache_cap = 7;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        cache_len = 7;
    }

    /* If the desired index is within the cache, return it */
    if ((idx_t)cache_len > n) return cache[n];

    /* Grow cache to accommodate index n */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t v = 5 * cache[i - 1];
        v -= 3 * cache[i - 2];
        v -= 9 * cache[i - 3];
        v += 16 * cache[i - 4];
        v -= 16 * cache[i - 6];
        v -= 16 * cache[i - 7];
        cache[i] = v;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
