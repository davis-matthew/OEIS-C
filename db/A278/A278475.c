/*
 * A278475  a(n) = floor(phi^7*a(n-1)) for n>0, a(0) = 1, where phi is the
 *          golden ratio (A001622).
 * https://oeis.org/A278475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - 28*a(n-2) - a(n-3), with a(0)=1, a(1)=29,
 *            a(2)=841
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)841;

    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* allocate space for the first three terms if needed */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
        cache[2] = (seq_t)841;
    }

    /* if we already have the value, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* grow the cache to size n+1 and compute missing terms */
    size_t new_len = (size_t)(n + 1);
    if (new_len > (size_t)cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* compute all terms from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)(30 * cache[i - 1] - 28 * cache[i - 2] - cache[i - 3]);
        }
        cache_len = new_len;
    }

    return cache[n];
}
