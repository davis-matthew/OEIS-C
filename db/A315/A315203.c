/*
 * A315203  Coordination sequence Gal.6.321.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) initial
 *            values: a(0)=1, a(1)=6, a(2)=10, a(3)=14, a(4)=20, a(5)=24,
 *            a(6)=28, a(7)=34, a(8)=38, a(9)=42, a(10)=48
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t base[11] = {
        1, 6, 10, 14, 20, 24, 28, 34, 38, 42, 48
    };

    /* initialise cache on first call */
    if (!cache)
    {
        cache_size = 11;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed */
        for (size_t i = 0; i < 11; ++i)    /* copy initial values */
            cache[i] = base[i];
    }

    /* if requested index already cached, return it */
    if (n < cache_size)
        return cache[(size_t)n];

    /* grow cache to contain the required index */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return 0;             /* allocation failed */
    cache = new_cache;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t v = (seq_t)2 * cache[i-1] - (seq_t)2 * cache[i-2] +
                  (seq_t)2 * cache[i-3] - (seq_t)2 * cache[i-4] +
                  (seq_t)2 * cache[i-5] - (seq_t)2 * cache[i-6] +
                  (seq_t)2 * cache[i-7] - (seq_t)2 * cache[i-8] +
                  (seq_t)2 * cache[i-9] - cache[i-10];
        cache[i] = v;
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
