/*
 * A310648  Coordination sequence Gal.5.253.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) for n > 7 a(0)=1, a(1)=4,
 *            a(2)=6, a(3)=14, a(4)=20, a(5)=20, a(6)=28, a(7)=28
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed values */
    static idx_t cache_size = 0;     /* number of elements currently stored */

    /* Expand cache to hold values up to n (inclusive) */
    if (cache_size <= n) {
        idx_t desired = (n >= 7) ? n + 1 : 8;   /* at least 8 for the base values */
        idx_t old_size = cache_size;
        seq_t *tmp = (seq_t *)realloc(cache, desired * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failed: return 0 as sentinel */
        cache = tmp;
        cache_size = desired;

        /* Initialise the base values the first time the cache is created */
        if (old_size == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)4;
            cache[2] = (seq_t)6;
            cache[3] = (seq_t)14;
            cache[4] = (seq_t)20;
            cache[5] = (seq_t)20;
            cache[6] = (seq_t)28;
            cache[7] = (seq_t)28;
        }

        /* Compute any new values required by the recurrence */
        for (idx_t i = old_size; i <= n; ++i) {
            if (i < 8) continue;  /* base values already set */
            cache[i] = cache[i-1] + cache[i-5] - cache[i-6];
        }
    }

    return cache[n];
}
