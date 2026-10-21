/*
 * A284604  Quadratic recurrence: a(n+2) = a(n+1)^2 + a(n)^2 + 1, with a(0) =
 *          a(1) = 1.
 * https://oeis.org/A284604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1, a(n+2) = a(n+1)^2 + a(n)^2 + 1 (OEIS
 *            A284604)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Code uses a simple memoised recurrence in O(n) time and O(n) memory. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holds all computed terms up to the highest requested index. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* If the requested term is already in the cache, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Initialise cache for the first two terms if it has not been done yet. */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * (size_t)cache_len);
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* If the cache is not large enough, extend it and compute new terms. */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (size_t)new_len);
        if (!new_cache) abort();
        cache = new_cache;
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            cache[i] = t1 * t1 + t2 * t2 + (seq_t)1;
        }
        cache_len = new_len;
    }

    return cache[n];
}
