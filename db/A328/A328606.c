/*
 * A328606  Expansion of (1 + 9*x) / (1 - 2*x - 11*x^2).
 * https://oeis.org/A328606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 11, a(n) = 2*a(n-1) + 11*a(n-2) for n > 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;

    /* ensure we have computed up to n */
    if ((size_t)n >= cache_size) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < needed) new_cap *= 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = new_cache;
            cache_cap = new_cap;
        }
        for (size_t i = cache_size; i < needed; ++i) {
            if (i == 0)        cache[i] = (seq_t)1;
            else if (i == 1)   cache[i] = (seq_t)11;
            else
                cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)11 * cache[i - 2];
        }
        cache_size = needed;
    }
    return cache[(size_t)n];
}
