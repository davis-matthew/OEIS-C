/*
 * A313707  Coordination sequence Gal.6.198.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-4) + 2*a(n-5) * - a(n-6) - a(n-8)
 *            + 2*a(n-9) - a(n-10) for n > 10 * Initial values: * a(0)=1,
 *            a(1)=5, a(2)=10, a(3)=15, a(4)=19, a(5)=25, a(6)=30, * a(7)=35,
 *            a(8)=41, a(9)=45, a(10)=50 * The function memoises results so it
 *            can be called with any n in any order
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t a_init[11] = {
    1, 5, 10, 15, 19, 25, 30, 35, 41, 45, 50
};

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 10;  /* last index that has a value */

    /* initialise cache on first call */
    if (!cache) {
        cache_size = 11;                     /* indices 0..10 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure -> 0 */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = a_init[i];
    }

    /* grow cache if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size ? cache_size : 1;
        while (new_size <= n)
            new_size <<= 1;                  /* double until it fits */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute new values up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* apply recurrence; indices i-1 .. i-10 are already defined */
        cache[i] =  (2 * cache[i-1]) - cache[i-2] - cache[i-4]
                  + (2 * cache[i-5]) - cache[i-6] - cache[i-8]
                  + (2 * cache[i-9]) - cache[i-10];
    }
    last_computed = n > last_computed ? n : last_computed;

    return cache[n];
}
