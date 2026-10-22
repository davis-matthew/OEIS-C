/*
 * A383757  Expansion of 1/Product_{k=0..3} (1 - 3^k * 4^(3-k) * x).
 * https://oeis.org/A383757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 175*a(n-1) - 11100*a(n-2) + 302400*a(n-3) -
 *            2985984*a(n-4) with a(0)=1, a(1)=175, a(2)=19525, a(3)=1776775
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t last_filled = 0;

    /* Ensure capacity for at least n+1 entries */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap;
        while (new_cap < needed) new_cap <<= 1;
#if defined(__cplusplus)
        seq_t *new_cache = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            delete [] cache;
        }
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* out of memory */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
#endif
        cache_cap = new_cap;
    }

    /* Initialise initial terms once */
    if (last_filled == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)175;
        cache[2] = (seq_t)19525;
        cache[3] = (seq_t)1776775;
        last_filled = 4;
    }

    /* Return cached value if already computed */
    if ((size_t)n < last_filled)
        return cache[n];

    /* Compute remaining terms up to n */
    for (size_t i = last_filled; i <= (size_t)n; ++i) {
        seq_t t = cache[i-1];
        t *= (seq_t)175;
        t -= (seq_t)11100 * cache[i-2];
        t += (seq_t)302400 * cache[i-3];
        t -= (seq_t)2985984 * cache[i-4];
        cache[i] = t;
    }
    last_filled = (size_t)n + 1;

    return cache[n];
}
