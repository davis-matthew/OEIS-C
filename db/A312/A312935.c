/*
 * A312935  Coordination sequence Gal.4.105.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) with initial values a(0)=1,
 *            a(1)=4, a(2)=9, a(3)=13, a(4)=18, a(5)=22, a(6)=28, a(7)=32,
 *            a(8)=37, a(9)=41, a(10)=46, a(11)=50, a(12)=54 Recurrence holds
 *            for n > 12
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store computed terms up to the largest
       n requested so far.  It is lazily initialised and expanded
       on demand. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initial values for indices 0..12 */
    static const seq_t init_vals[13] = {
        1ULL, 4ULL, 9ULL, 13ULL, 18ULL, 22ULL,
        28ULL, 32ULL, 37ULL, 41ULL, 46ULL, 50ULL, 54ULL
    };

    /* Allocate and initialise cache if we have never used it. */
    if (cache == NULL) {
        cache_size = 13;                     /* indices 0..12 inclusive */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                /* return 0 on memory failure */
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
    }

    /* If the requested index n is outside our current cache, grow it. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;              /* at least up to n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* return 0 on memory failure */
        cache = tmp;

        /* Compute any new entries using the recurrence. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 12) { /* already initialised */
                continue;
            }
            cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
        }

        cache_size = new_size;
    }

    return cache[n];
}
