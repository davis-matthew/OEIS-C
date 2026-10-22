/*
 * A310430  Coordination sequence Gal.3.41.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 4*a(n-3) - 3*a(n-4) + 2*a(n-5) -
 *            a(n-6) for n>6; Initial values: a(0)=1, a(1)=4, a(2)=10,
 *            a(3)=15, a(4)=18, a(5)=23, a(6)=30
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static const seq_t base[7] = {1, 4, 10, 15, 18, 23, 30};

seq_t a(idx_t n)
{
    /* If cache not yet initialized, create and fill base values */
    if (cache_cap == 0) {
        cache = (seq_t*)malloc(7 * sizeof(seq_t));
        if (!cache) return 0; /* fallback */
        for (size_t i = 0; i < 7; ++i) cache[i] = base[i];
        cache_cap = 7;
    }

    size_t idx = (size_t)n;
    if (idx < cache_cap) return cache[idx];
    /* Grow cache to accommodate up to idx */
    cache = (seq_t*)realloc(cache, (idx + 1) * sizeof(seq_t));
    if (!cache) return 0; /* fallback */
    /* Compute new values using the recurrence */
    for (size_t i = cache_cap; i <= idx; ++i) {
        cache[i] = (seq_t)2 * cache[i-1] - (seq_t)3 * cache[i-2]
                 + (seq_t)4 * cache[i-3] - (seq_t)3 * cache[i-4]
                 + (seq_t)2 * cache[i-5] -      cache[i-6];
    }
    cache_cap = idx + 1;
    return cache[idx];
}
