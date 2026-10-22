/*
 * A311007  Coordination sequence Gal.4.19.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-5) - a(n-6) + a(n-7) -
 *            a(n-8) for n > 11 Initial values: a(0)=1, a(1)=4, a(2)=8,
 *            a(3)=10, a(4)=16, a(5)=22, a(6)=24, a(7)=30, a(8)=34, a(9)=38,
 *            a(10)=42, a(11)=44
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* number of allocated slots */
    static size_t cache_len  = 0;   /* number of slots actually used (next free index) */

    /* grow the cache when necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initial values */
    static const seq_t init[12] = {
        1, 4, 8, 10, 16, 22, 24, 30, 34, 38, 42, 44
    };

    /* compute missing terms up to n (increasing order, memoised) */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 12) {
            cache[i] = init[i];
        } else {
            seq_t val = cache[i-1] - cache[i-2];
            val += cache[i-3];
            val += cache[i-5];
            val -= cache[i-6];
            val += cache[i-7];
            val -= cache[i-8];
            cache[i] = val;
        }
    }
    cache_len = (size_t)(n + 1);
    return cache[n];
}
