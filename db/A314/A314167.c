/*
 * A314167  Coordination sequence Gal.6.638.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-4) + a(n-6) + a(n-7) - a(n-8),
 *            a(0) = 1, a(1) = 5, a(2) = 11, a(3) = 16, a(4) = 22, a(5) = 28,
 *            a(6) = 34, a(7) = 40, a(8) = 46
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size */
    static idx_t cached_n   = 0;   /* highest n computed */

    /* initialise cache with the first 9 known values */
    if (!cache) {
        cache_size = 9;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)28;
        cache[6] = (seq_t)34;
        cache[7] = (seq_t)40;
        cache[8] = (seq_t)46;
        cached_n = 8;
    }

    /* if n is already computed, return it */
    if (n <= cached_n)
        return cache[(size_t)n];

    /* extend cache storage if necessary */
    idx_t needed = n + 1;
    if (needed > cache_size) {
        seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        for (idx_t i = 0; i <= cached_n; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_size = needed;
    }

    /* compute missing terms using the recurrence */
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        seq_t term = cache[k - 1] + cache[k - 2]
                   - (seq_t)2 * cache[k - 4]
                   + cache[k - 6] + cache[k - 7]
                   - cache[k - 8];
        cache[k] = term;
    }

    cached_n = n;
    return cache[(size_t)n];
}
