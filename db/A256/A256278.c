/*
 * A256278  a(0)=1, a(1)=2, a(n) = 31*a(n-1) - 29*a(n-2).
 * https://oeis.org/A256278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) - 29*a(n-2); a(0)=1, a(1)=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure base values are cached */
    if (cache_cap == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_cap = 2;
    }

    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        for (size_t i = cache_cap; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)31 * cache[i - 1] - (seq_t)29 * cache[i - 2];
        }
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
