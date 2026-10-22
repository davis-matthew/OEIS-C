/*
 * A315563  Coordination sequence Gal.5.320.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-3) - a(n-6) for n > 6; initial values: a(0)=1,
 *            a(1)=6, a(2)=11, a(3)=18, a(4)=23, a(5)=28, a(6)=36
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms   */
static idx_t cache_size = 0;         /* number of elements in cache */

seq_t a(idx_t n) {
    /* If we already have the value return it immediately */
    if (n < cache_size) return cache[n];

    /* Need to enlarge the cache to fit up to index n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {                    /* allocation failed – return 0 (unlikely) */
        return 0;
    }
    cache = tmp;

    /* Compute and store the terms that are not yet in the cache */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)        cache[i] = 1;
        else if (i == 1)   cache[i] = 6;
        else if (i == 2)   cache[i] = 11;
        else if (i == 3)   cache[i] = 18;
        else if (i == 4)   cache[i] = 23;
        else if (i == 5)   cache[i] = 28;
        else if (i == 6)   cache[i] = 36;
        else               cache[i] = 2 * cache[i - 3] - cache[i - 6];
    }
    cache_size = new_size;

    return cache[n];
}
