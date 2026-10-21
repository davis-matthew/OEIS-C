/*
 * A293007  Expansion of 2*x^2 / (1-2*x-2*x^2).
 * https://oeis.org/A293007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 0, a(2) = 2, a(n) = 2*a(n-1)+2*a(n-2) for n>=3
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of initialized entries   */
    static size_t cache_cap = 0;   /* current capacity of cache array */

    size_t idx = (size_t)n;

    /* First call: allocate minimal cache space and set base cases */
    if (cache == NULL) {
        cache_cap = 8;                   /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out‑of‑memory guard */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache_len = 3;
    }

    /* If requested index already cached, return it directly */
    if (idx < cache_len)
        return cache[idx];

    /* Grow the cache array if needed */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= idx) new_cap <<= 1;      /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;         /* out‑of‑memory guard */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to idx, storing each in cache */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)0;
        } else if (i == 2) {
            cache[i] = (seq_t)2;
        } else {
            seq_t t1 = mul_small(cache[i-1], 2u);
            seq_t t2 = mul_small(cache[i-2], 2u);
            cache[i] = t1 + t2;
        }
    }
    cache_len = idx + 1;

    return cache[idx];
}
