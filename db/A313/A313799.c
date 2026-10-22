/*
 * A313799  Coordination sequence Gal.3.53.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0..8) = 1, 5, 10, 15, 21, 27, 33, 38, 43 * a(n) = 2*a(n-1) -
 *            a(n-2) - a(n-3) + 2*a(n-4) * - a(n-5) - a(n-6) + 2*a(n-7) -
 *            a(n-8) for n >= 9
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;   /* pointer to allocated array */
    static idx_t cache_size  = 0;      /* number of values stored */
    static size_t cache_cap  = 0;      /* size of allocated storage */

    /* Ensure enough space for indices up to n. */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        /* realloc in a way that is acceptable in both C and C++. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) {
            abort();   /* allocation failure – terminate promptly */
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Base values for 0 .. 8 */
    static const seq_t init[9] = {
        (seq_t)1,  (seq_t)5,  (seq_t)10, (seq_t)15, (seq_t)21,
        (seq_t)27, (seq_t)33, (seq_t)38, (seq_t)43
    };

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init[i];
        } else {
            cache[i] =  2 * cache[i-1]
                       - cache[i-2] - cache[i-3]
                       + 2 * cache[i-4]
                       - cache[i-5] - cache[i-6]
                       + 2 * cache[i-7]
                       - cache[i-8];
        }
    }
    cache_size = n + 1;

    return cache[n];
}
