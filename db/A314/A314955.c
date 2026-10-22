/*
 * A314955  Coordination sequence Gal.6.322.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12; Base values:
 *            a(0)=1, a(1)=5, a(2)=9, a(3)=14, a(4)=20, a(5)=25, a(6)=29,
 *            a(7)=34, a(8)=40, a(9)=45, a(10)=49, a(11)=54, a(12)=59
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoization cache */
static size_t cache_size = 0;    /* current size of cache */

/* initialise base values once */
static const seq_t base_vals[13] = {
    1, 5, 9, 14, 20, 25, 29, 34, 40, 45, 49, 54, 59
};

seq_t a(idx_t n)
{
    /* initialise cache if first call */
    if (!cache) {
        cache_size = 13;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = base_vals[i];
    }

    /* if already computed, return it */
    if ((size_t)n < cache_size)
        return cache[n];

    /* extend cache up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – return 0 as a safe default (should not happen) */
        return 0;
    }
    cache = tmp;

    /* compute missing values using the recurrence */
    for (size_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
    }
    cache_size = new_size;

    return cache[n];
}
