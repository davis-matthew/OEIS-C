/*
 * A364105  Expansion of Sum_{k>0} k * x^(2*k) / (1 - x^(5*k-1)).
 * https://oeis.org/A364105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Σ_{d|5*n-2, d≡4 (mod 5)} (d+1)
 * Range:     u8 n <= 13, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"  /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* Compute m = 5*n - 2 as a seq_t to avoid overflow. */
    seq_t m = (seq_t)5 * n - (seq_t)2;
    seq_t sum = 0;

    /* The largest possible divisor to search is sqrt(m). */
    seq_t lim = floor_sqrt(m);

    for (seq_t i = 1; i <= lim; ++i)
    {
        if (m % i == 0)
        {
            seq_t d = i;
            seq_t e = m / i;

            if (d % 5 == 4)
                sum += d + 1;

            if (e != d && e % 5 == 4)
                sum += e + 1;
        }
    }

    return sum / 5;
}
