/*
 * A315517  Coordination sequence Gal.6.208.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-4) + a(n-6) + a(n-7) - a(n-8) for
 *            n > 8 Initial values (for n = 0 … 8): a(0)=1, a(1)=6, a(2)=11,
 *            a(3)=17, a(4)=22, a(5)=27, a(6)=32, a(7)=37, a(8)=42
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[9] = {1,6,11,17,22,27,32,37,42};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized values up to cached_n */
    static idx_t   cached_n = 0;  /* largest index computed so far */

    /* If we need more space, allocate (or grow) the cache */
    if (cache == NULL || n > cached_n)
    {
        idx_t new_size = n + 1;                            /* include n itself   */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)                                          /* allocation failed */
            return (seq_t)0;
        cache = tmp;

        /* Initialize the fixed initial segment 0 … min(n,8) */
        idx_t up = n < 8 ? n : 8;
        for (idx_t i = 0; i <= up; ++i)
            cache[i] = init_vals[i];

        /* We have values up to 'up' ready */
        cached_n = up;

        /* If we still need more, apply the recurrence for indices ≥ 9 */
        if (n > 8)
        {
            for (idx_t i = 9; i <= n; ++i)
                cache[i] = cache[i-1] + cache[i-2]
                        - 2*cache[i-4] + cache[i-6]
                        + cache[i-7] - cache[i-8];

            cached_n = n;
        }
    }

    return cache[n];
}
