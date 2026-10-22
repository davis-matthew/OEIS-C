/*
 * A313241  Coordination sequence Gal.5.86.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10 (a(0) = 1, a(1) = 4,
 *            a(2) = 9, a(3) = 15, a(4) = 20, a(5) = 24, a(6) = 29, a(7) = 35,
 *            a(8) = 40, a(9) = 44, a(10) = 48)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached sequence values   */
    static size_t cache_cap = 0;     /* allocated size           */
    static idx_t  cache_max = (idx_t)-1; /* highest index cached */

    /* initial values for n = 0 .. 10 */
    static const seq_t init_vals[11] = {
        1, 4, 9, 15, 20, 24, 29, 35, 40, 44, 48
    };

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 11;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed (unlikely) */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init_vals[i];
        cache_max = 10;
    }

    /* extend cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
        /* compute new values */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1) + a(i-9) - a(i-10)   (i > 10) */
            cache[i] = cache[i - 1] + cache[i - 9] - cache[i - 10];
        }
        cache_max = n;
    }

    return cache[n];
}
