/*
 * A276139  Series expansion of (1 + 2x + 4x^2)/(1 - 3x - 5x^2).
 * https://oeis.org/A276139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) (n > 2); initial values: a(0)=1,
 *            a(1)=5, a(2)=24
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of entries stored */
    static size_t cache_cap   = 0;   /* capacity of the allocated array */

    /* Guard against overflow when converting idx_t to size_t */
    if (n > (idx_t)(SIZE_MAX - 1))
        return (seq_t)0;

    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);   /* allocation failed */
        cache     = new_cache;
        cache_cap = needed;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)5;
        else if (i == 2)
            cache[i] = (seq_t)24;
        else
            cache[i] = ((seq_t)3) * cache[i-1] + ((seq_t)5) * cache[i-2];
    }
    cache_size = needed;
    return cache[n];
}
