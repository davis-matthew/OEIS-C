/*
 * A292292  Number of vertices of type C at level n of the hyperbolic Pascal
 *          pyramid.
 * https://oeis.org/A292292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 37*a(n-2) + 37*a(n-3) - 12*a(n-4) + a(n-5) ,
 *            n ≥ 6 a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=3, a(5)=9
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 26, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t  cache_last = 5;
    static size_t cache_cap  = 0;

    static const seq_t base[6] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)3, (seq_t)9
    };

    /* Return the first six terms directly */
    if (n < 6) return base[n];

    /* Create the cache on first use */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < 6; ++i) cache[i] = base[i];
    }

    /* Grow the cache if the requested index is beyond its current size */
    if ((idx_t)(cache_cap - 1) < n) {
        size_t needed   = (size_t)n + 1;
        size_t new_cap  = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute the missing terms up to n using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t val = 12 * cache[i-1];
        val -= 37 * cache[i-2];
        val += 37 * cache[i-3];
        val -= 12 * cache[i-4];
        val +=        cache[i-5];
        cache[i] = val;
    }
    cache_last = n;

    return cache[n];
}
