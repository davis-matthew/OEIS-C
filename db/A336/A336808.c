/*
 * A336808  a(n) = (n!)^2 * Sum_{k=0..n} 5^(n-k) / (k!)^2.
 * https://oeis.org/A336808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n^2*a(n-1) + 1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to avoid recomputing same values for multiple calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries in cache */

    if (!cache) {
        /* initialise cache for a(0) */
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 1;
        cache_len = 1;
    }

    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t prev = cache[i - 1];
            seq_t factor = (seq_t)5 * (seq_t)(idx_t)i * (seq_t)(idx_t)i;
            cache[i] = prev * factor + 1;
        }
        cache_len = new_len;
    }

    return cache[n];
}
