/*
 * A250121  Crystal ball sequence for planar net 3.3.3.3.6.
 * https://oeis.org/A250121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) Initial
 *            values: a(0)=1, a(1)=6, a(2)=15, a(3)=30, a(4)=49, a(5)=73,
 *            a(6)=102
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure the cache is initialized with the first 7 terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 7);
        if (!cache) abort(); /* out of memory */
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 15;
        cache[3] = 30;
        cache[4] = 49;
        cache[5] = 73;
        cache[6] = 102;
        cache_len = 7;
    }

    /* If n is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache to contain up to n */
    if (cache_len <= (size_t)n) {
        size_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!new_cache) abort(); /* out of memory */
        cache = new_cache;

        /* Compute all missing terms from cache_len to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t v = 0;
            v += 2 * cache[i - 1];
            v -= cache[i - 2];
            v += cache[i - 5];
            v -= 2 * cache[i - 6];
            v += cache[i - 7];
            cache[i] = v;
        }
        cache_len = new_len;
    }

    return cache[n];
}
