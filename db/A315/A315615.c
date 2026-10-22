/*
 * A315615  Coordination sequence Gal.6.609.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 4*a(n-3) - 5*a(n-4) + 6*a(n-5) -
 *            7*a(n-6) + 8*a(n-7) - 7*a(n-8) + 6*a(n-9) - 5*a(n-10) +
 *            4*a(n-11) - 3*a(n-12) + 2*a(n-13) - a(n-14) for n > 14 Initial
 *            values: a(0)=1, a(1)=6, a(2)=12, a(3)=15, a(4)=18, a(5)=21,
 *            a(6)=24, a(7)=33, a(8)=42, a(9)=45, a(10)=48, a(11)=51,...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values up to a(14) */
    static const seq_t init[15] = {
        1, 6, 12, 15, 18, 21, 24, 33, 42, 45, 48, 51, 54, 57, 60
    };
    if (n < 15) /* direct lookup for the known initial segment */
        return init[(size_t)n];

    /* Coefficients of the recurrence (length 14) */
    static const int coeff[14] = {
        2, -3, 4, -5, 6, -7, 8, -7, 6, -5, 4, -3, 2, -1
    };

    /* Cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with the first 15 terms */
    if (cache == NULL) {
        cache_size = 15;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
    }

    /* Grow cache up to required n */
    if (n >= cache_size) {
        size_t old_size = cache_size;
        cache_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        for (size_t i = old_size; i <= (size_t)n; ++i) {
            seq_t pos = 0;  /* sum of positive‑coefficient terms */
            seq_t neg = 0;  /* sum of negative‑coefficient terms (absolute value) */
            for (int k = 1; k <= 14; ++k) {
                seq_t v = cache[i - k];
                int c = coeff[k - 1];
                if (c > 0)
                    pos = pos + (v * (seq_t)c);
                else
                    neg = neg + (v * (seq_t)(-c));
            }
            /* pos >= neg for every step, so subtraction is safe */
            cache[i] = pos - neg;
        }
    }

    return cache[(size_t)n];
}
