/*
 * A310651  Coordination sequence Gal.6.548.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-7) - a(n-14) for n > 15; Initial values: a(0)=1,
 *            a(1)=4, a(2)=6, a(3)=14, a(4)=20, a(5)=26, a(6)=28, a(7)=30,
 *            a(8)=40, a(9)=38, a(10)=48, a(11)=54, a(12)=60, a(13)=60,
 *            a(14)=62, a(15)=74
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Note: The sequence is linear for n > 15, so unsigned computation
   will not underflow or overflow for n up to 10,000. */

static const seq_t init_values[16] = {
    1, 4, 6, 14, 20, 26, 28, 30,
    40, 38, 48, 54, 60, 60, 62, 74
};

seq_t a(idx_t n)
{
    /* Static cache to accommodate arbitrary order of calls. */
    static seq_t *cache = NULL;
    static idx_t cap = 0;               /* size of the cache */

    /* Ensure the cache holds at least indices up to n. */
    if (cap <= n) {
        idx_t newcap = n + 1;
        seq_t *newcache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newcache) abort();        /* memory failure */
        cache = newcache;

        /* Initialize newly allocated entries. */
        for (idx_t i = cap; i <= n; ++i) {
            if (i < 16) {
                cache[i] = init_values[i];
            } else {
                cache[i] = 2 * cache[i - 7] - cache[i - 14];
            }
        }
        cap = newcap;
    }

    return cache[n];
}
