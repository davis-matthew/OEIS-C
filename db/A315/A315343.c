/*
 * A315343  Coordination sequence Gal.5.304.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10 Initial values:
 *            a(0)=1, a(1)=6, a(2)=10, a(3)=16, a(4)=22, a(5)=28, a(6)=34,
 *            a(7)=40, a(8)=44, a(9)=50, a(10)=56
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap   = 0;   /* allocated size of cache */
    static idx_t   cache_max  = (idx_t)-1; /* highest index computed so far */

    /* base values for n = 0..10 */
    static const seq_t base[11] = {
        (seq_t)1,  (seq_t)6,  (seq_t)10, (seq_t)16,
        (seq_t)22, (seq_t)28, (seq_t)34, (seq_t)40,
        (seq_t)44, (seq_t)50, (seq_t)56
    };

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 11;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0; /* out of memory; caller should handle */
        for (int i = 0; i < 11; ++i)
            cache[i] = base[i];
        cache_max = 10;
    }

    /* Return value if already computed */
    if ((idx_t)n <= cache_max)
        return cache[(size_t)n];

    /* Grow cache to accommodate new index 'n' */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0; /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from cache_max+1 up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* i must be >= 11 here; recurrence is safe */
        cache[(size_t)i] = cache[(size_t)(i - 1)] +
                           cache[(size_t)(i - 9)] -
                           cache[(size_t)(i - 10)];
    }
    cache_max = n;
    return cache[(size_t)n];
}
