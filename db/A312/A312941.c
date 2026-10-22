/*
 * A312941  Coordination sequence Gal.5.14.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-6) - a(n-7) for n > 7 Initial values:
 *            a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=18, a(5)=23, a(6)=27,
 *            a(7)=31
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* dynamically grown cache */
static size_t cache_size = 0;   /* number of elements actually computed */
static size_t cache_cap  = 0;   /* capacity of the allocated array */

seq_t a(idx_t n)
{
    /* Small set of initial values for indices 0..7 */
    static const seq_t init_vals[8] = { 1, 4, 9, 13, 18, 23, 27, 31 };
    if ((size_t)n < 8) return init_vals[n];

    /* Initialise the cache if it does not yet exist */
    if (!cache)
    {
        cache_cap  = (size_t)n + 1;
        if (cache_cap < 16) cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed – 0 is a sensible sentinel */
        cache_size = 0;
    }

    /* Ensure the cache is large enough to hold index n */
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed – 0 is a sensible sentinel */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute and store any values that are missing up to index n */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        if (i < 8)
            cache[i] = init_vals[i];
        else
            cache[i] = cache[i-1] + cache[i-6] - cache[i-7];
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
