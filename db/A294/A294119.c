/*
 * A294119  Expansion of e.g.f.: exp(2*((1+x)^2 - 1)).
 * https://oeis.org/A294119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(n)=4*a(n-1)+4*(n-1)*a(n-2) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Static cache to store already-computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size in terms of seq_t */
    static idx_t cached_up_to = 0; /* highest index computed so far */

    /* Initial allocation */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cached_up_to = 1;
    }

    /* Ensure the cache is large enough for n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < 4) new_cap = 4;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If we already have the value, return it */
    if (n <= cached_up_to) return cache[n];

    /* Compute any missing terms up to n */
    seq_t four = (seq_t)4;
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t term1 = cache[i-1];
        seq_t term2 = cache[i-2];
        seq_t value = four * term1 + four * (seq_t)(i-1) * term2;
        cache[i] = value;
    }
    cached_up_to = n;

    return cache[n];
}
