/*
 * A315701  Coordination sequence Gal.4.136.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-7) - a(n-8) for n > 8 Initial values:
 *            a(0)=1, a(1)=6, a(2)=12, a(3)=17, a(4)=23, a(5)=28, a(6)=34,
 *            a(7)=40, a(8)=46
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"


static seq_t *cache = NULL;     /* array holding computed terms */
static size_t cache_cap = 0;    /* current capacity of cache (number of slots) */
static idx_t  known_max = 0;    /* highest index already computed */

/* Ensure cache has room for indices up to n (inclusive) */
static void extend_cache(idx_t n)
{
    if (n >= (idx_t)cache_cap)
        ensure_cache(n, &cache, &cache_cap);
}

/* Initialise the cache with the first nine terms */
static void init_cache(void)
{
    if (!cache)
    {
        extend_cache(8);   /* needs indices 0..8 */
        cache[0] = 1;  cache[1] = 6;  cache[2] = 12;
        cache[3] = 17; cache[4] = 23; cache[5] = 28;
        cache[6] = 34; cache[7] = 40; cache[8] = 46;
        known_max = 8;
    }
}

/* Compute the coordination sequence term for index n */
seq_t a(idx_t n)
{
    init_cache();           /* make sure base values exist */
    if (n <= known_max)    /* already computed */
        return cache[n];

    extend_cache(n);        /* enlarge array as needed */

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = known_max + 1; i <= n; ++i)
    {
        /* a(i) = a(i-1) + a(i-7) - a(i-8)  (valid for i > 8) */
        cache[i] = cache[i-1] + cache[i-7] - cache[i-8];
    }
    known_max = n;
    return cache[n];
}
