/*
 * A267652  a(n) = 4*a(n - 1) + 4*a(n - 2) for n>1, a(0)=2, a(1)=3.
 * https://oeis.org/A267652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=3, a(n)=4*a(n-1)+4*a(n-2) for n>1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

seq_t a(idx_t n) {
    /* Ensure the cache is initialized with the first two terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* fallback on allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache_size = 2;
    }

    /* If the requested index is already cached, return it */
    if ((size_t)n < cache_size) {
        return cache[(size_t)n];
    }

    /* Extend the cache if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t newcap = cache_cap;
        while (newcap < needed) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* fallback on reallocation failure */
        cache = tmp;
        cache_cap = newcap;
    }

    /* Compute new terms using the recurrence */
    const seq_t mul4 = (seq_t)4;
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] * mul4 + cache[i-2] * mul4;
    }

    cache_size = needed;
    return cache[(size_t)n];
}
