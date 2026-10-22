/*
 * A315707  Coordination sequence Gal.5.295.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) for n > 6 Initial values:
 *            a(0)=1, a(1)=6, a(2)=12, a(3)=17, a(4)=23, a(5)=29, a(6)=35 This
 *            function memoises the values so that calls with arbitrary n are
 *            fast
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoised sequence values */
static size_t cache_len = 0;       /* number of elements stored so far (0 means empty) */
static size_t cache_cap = 0;       /* allocated capacity of cache */

static const seq_t base[7] = {1, 6, 12, 17, 23, 29, 35};

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n < 7)
        return base[(size_t)n];

    /* Allocate or enlarge the cache if needed */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) {
            abort();              /* allocation failure */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 7) {
            cache[i] = base[i];
        } else {
            cache[i] = cache[i-1] + cache[i-5] - cache[i-6];
        }
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
