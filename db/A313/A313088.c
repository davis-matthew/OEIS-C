/*
 * A313088  Coordination sequence Gal.6.120.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 3*a(n-3) - a(n-4) - a(n-6) +
 *            3*a(n-7) - 4*a(n-8) + 3*a(n-9) - a(n-10) initial values
 *            (n=0..10): a(0)=1, a(1)=4, a(2)=9, a(3)=14, a(4)=19, a(5)=23,
 *            a(6)=27, a(7)=31, a(8)=35, a(9)=40, a(10)=45 The recurrence is
 *            valid for all n > 10
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values up to a(10) */
    static const seq_t init[11] = {
        1, 4, 9, 14, 19, 23, 27, 31, 35, 40, 45
    };

    /* return the known values directly for small n      */
    if (n < 11)
        return init[n];

    /* static cache: memoised sequence values             */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements stored (actually >= 11 when allocated) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* initialise the cache the first time */
    if (cache == NULL) {
        cache_cap = 11;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)   /* allocation failed – return a sentinel value           */
            return 0;
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        cache_len = 11;
    }

    /* grow the cache if it does not yet contain a(n)   */
    if (cache_len <= n) {
        idx_t need = n + 1;
        if (need > cache_cap) {
            idx_t new_cap = cache_cap;
            while (new_cap < need)
                new_cap <<= 1;                 /* grow by powers of two */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)   /* reallocation failed – return a sentinel value      */
                return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute the sequence terms sequentially        */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = 3*a(i-1) - 4*a(i-2) + 3*a(i-3) - a(i-4)
                      - a(i-6) + 3*a(i-7) - 4*a(i-8) + 3*a(i-9) - a(i-10) */
            seq_t pos = cache[i-1] * (seq_t)3
                      + cache[i-3] * (seq_t)3
                      + cache[i-7] * (seq_t)3
                      + cache[i-9] * (seq_t)3;

            seq_t neg = cache[i-2] * (seq_t)4
                      + cache[i-4]
                      + cache[i-6]
                      + cache[i-8] * (seq_t)4
                      + cache[i-10];

            cache[i] = pos - neg;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
