/*
 * A286062  a(n) = 2*a(n-1) + a(n-2) - a(n-3), where a(0) = 2, a(1) = 3, a(2)
 *          = 6.
 * https://oeis.org/A286062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3), a(0) = 2, a(1) = 3, a(2) = 6
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cache[0 .. cache_len-1] holds computed terms */
    static idx_t cache_len = 0;      /* number of valid elements in cache */

    /* initialise cache with the first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;        /* out of memory */
        cache[0] = 2;
        cache[1] = 3;
        cache[2] = 6;
    }

    /* if the requested term is already known, return it */
    if (n < cache_len)
        return cache[n];

    /* grow the cache to accommodate n */
    idx_t new_len = cache_len;
    while (new_len <= n)
        new_len <<= 1;              /* double until we cover n */

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;              /* out of memory */
    cache = tmp;

    /* compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 2 * cache[i-1] + cache[i-2] - cache[i-3];
    }

    cache_len = n + 1;
    return cache[n];
}
