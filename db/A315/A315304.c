/*
 * A315304  Coordination sequence Gal.5.84.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=10, a(3)=16, a(4)=20, a(5)=24, a(6)=28,
 *            a(7)=34, a(8)=38, a(9)=44, a(10)=50; a(n)=a(n-1)+a(n-9)-a(n-10)
 *            for n>10
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values for n = 0 .. 10 */
    static const seq_t init[11] = {
        (seq_t)1, (seq_t)6, (seq_t)10, (seq_t)16,
        (seq_t)20, (seq_t)24, (seq_t)28, (seq_t)34,
        (seq_t)38, (seq_t)44, (seq_t)50
    };

    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialize cache if not already */
    if (cache_len == 0) {
        cache_len = 11;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed, return 0 as safe fallback */
        for (idx_t i = 0; i < 11; ++i) {
            cache[i] = init[i];
        }
    }

    /* if requested index is already in cache, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* extend cache to hold up to n */
    seq_t *new_cache = cache;
    if (cache_len <= n) {
        new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */
        cache = new_cache;
        cache_len = n + 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = 11; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 9] - cache[i - 10];
    }

    return cache[n];
}
