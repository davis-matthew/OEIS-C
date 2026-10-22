/*
 * A315460  Coordination sequence Gal.6.234.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12, a(0)=1, a(1)=6,
 *            a(2)=11, a(3)=16, a(4)=21, a(5)=25, a(6)=29, a(7)=33, a(8)=38,
 *            a(9)=43, a(10)=48, a(11)=54, a(12)=60
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for 0 ≤ n ≤ 12 */
    static const seq_t init[] = {
        1, 6, 11, 16, 21, 25, 29, 33, 38, 43, 48, 54, 60
    };
    if (n < 13) return init[(size_t)n];

    /* Cache for computed terms */
    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;    /* allocated capacity */
    static idx_t    cache_len = 0;    /* next free index (size) */

    /* Allocate initial cache and copy base values */
    if (!cache) {
        cache_cap = 32;                    /* start with 32 slots */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;       /* abort on allocation failure */
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        cache_len = 13;                    /* first 13 terms stored */
    }

    /* Grow cache if requested index is beyond current capacity */
    if (n >= cache_cap) {
        while (n >= cache_cap) cache_cap <<= 1;   /* double until adequate */
        seq_t *newp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        if (!newp) return (seq_t)0;               /* abort on failure */
        cache = newp;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
    }
    cache_len = n + 1;          /* update cache size */
    return cache[n];
}
