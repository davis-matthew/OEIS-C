/*
 * A315420  Coordination sequence Gal.6.355.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12, with initial
 *            values a(0)=1, a(1)=6, a(2)=11, a(3)=15, a(4)=20, a(5)=26,
 *            a(6)=32, a(7)=38, a(8)=43, a(9)=47, a(10)=52, a(11)=58, a(12)=64
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored in cache */
    static size_t cache_cap = 0;  /* allocated capacity of cache */

    /* base values for 0 <= n <= 12 */
    static const seq_t base[13] = {
        1, 6, 11, 15, 20, 26, 32, 38, 43, 47, 52, 58, 64
    };

    /* First call: allocate cache and store base values */
    if (cache == NULL) {
        cache_cap = 64;  /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();   /* allocation failed */

        for (idx_t i = 0; i < 13; ++i)
            cache[i] = base[i];

        cache_len = 13;
    }

    /* If n < 13, the term is already stored in the cache */
    if (n < 13)
        return cache[n];

    /* Ensure the cache is large enough */
    if ((idx_t)cache_cap <= n) {
        /* grow capacity doubling until enough */
        while (cache_cap <= (size_t)n)
            cache_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
    }

    cache_len = n + 1;
    return cache[n];
}
