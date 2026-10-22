/*
 * A384648  Expansion of (1+2*x+x^2) / (1-x-5*x^2-2*x^3).
 * https://oeis.org/A384648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = a(n-1) + 5*a(n-2) + 2*a(n-3), with a(0) = 1, a(1) = 3,
 *            a(2) = 9
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise the cache with the first three terms. */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) return (seq_t)0;   /* safety, should not happen */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
    }

    /* Return already computed value if it is available. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend the cache up to n and fill in new terms. */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (tmp == NULL) return (seq_t)0;   /* safety, should not happen */
    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = a(i-1) + 5*a(i-2) + 2*a(i-3) */
        seq_t val = cache[i - 1] +
                    (seq_t)5 * cache[i - 2] +
                    (seq_t)2 * cache[i - 3];
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
