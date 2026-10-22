/*
 * A311824  Coordination sequence Gal.5.158.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 4*a(n-3) - 5*a(n-4) + 6*a(n-5) -
 *            7*a(n-6) + 8*a(n-7) - 8*a(n-8) + 8*a(n-9) - 8*a(n-10) +
 *            8*a(n-11) - 8*a(n-12) + 8*a(n-13) - 7*a(n-14) + 6*a(n-15) -
 *            5*a(n-16) + 4*a(n-17) - 3*a(n-18) + 2*a(n-19) - a(n-20) a(0) =
 *            1, a(1) = 4, a(2) = 8, a(3) = 13, a(4) = 17, a(5) = 21,...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[21] = {
        (seq_t)1, (seq_t)4, (seq_t)8, (seq_t)13, (seq_t)17,
        (seq_t)21, (seq_t)25, (seq_t)30, (seq_t)37, (seq_t)43,
        (seq_t)47, (seq_t)51, (seq_t)56, (seq_t)60, (seq_t)62,
        (seq_t)64, (seq_t)70, (seq_t)77, (seq_t)81, (seq_t)85,
        (seq_t)89
    };

    /* Coefficients for the recurrence (20 terms) */
    static const int coeffs[20] = {
        2, -3, 4, -5, 6, -7, 8, -8, 8, -8,
        8, -8, 8, -7, 6, -5, 4, -3, 2, -1
    };

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored (>= 21 after init) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Allocate or expand cache if necessary */
    if (!cache || n + 1 > cache_cap) {
        idx_t need = n + 1;
        idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 64;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* memory failure, return 0 (unlikely in contest) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise cache with the first 21 terms if not already done */
    if (cache_len == 0) {
        for (idx_t i = 0; i < 21; ++i)
            cache[i] = init[i];
        cache_len = 21;
    }

    if (n < cache_len)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        __int128 sum = 0;
        for (int j = 0; j < 20; ++j) {
            sum += (__int128)coeffs[j] * (__int128)cache[i - (j + 1)];
        }
        cache[i] = (seq_t)sum;
    }

    cache_len = n + 1;
    return cache[n];
}
