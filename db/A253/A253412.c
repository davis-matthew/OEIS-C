/*
 * A253412  Number of n-bit legal binary words with maximal set of 1s.
 * https://oeis.org/A253412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) - a(n-6) (n ≥ 6) Recurrence
 *            base: a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=4, a(5)=4, a(6)=7
 * Range:     u8 n <= 16, u16 n <= 33, u32 n <= 66, u64 n <= 131,
 *            u128 n <= 263, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization of past values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (elements) */
    static idx_t cache_size = 0;  /* number of values stored (next free index) */

    /* initialise base cases on the first call */
    if (cache_size == 0) {
        cache_cap = 7;  /* indices 0..6 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed (unlikely) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)4;
        cache[6] = (seq_t)7;
        cache_size = 7;
    }

    /* if the value is already computed, just return it */
    if (n < cache_size) return cache[n];

    /* extend the cache if the requested index is out of bounds */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing entries using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-2] + cache[i-3] + cache[i-4] - cache[i-6];
    }

    cache_size = n + 1;
    return cache[n];
}
