/*
 * A364065  Expansion of Sum_{k>0} k * x^(2*k-1) / (1 - x^(3*k-2)).
 * https://oeis.org/A364065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3) * sum_{d | (3*n-1), d==1 (mod 3)} (d+2)
 * Range:     u8 n <= 21, u16 n <= 5249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute m = 3*n - 1 */
    idx_t m = 3 * n - 1;

    seq_t sum = 0;

    /* floor_sqrt returns seq_t; use it as a bound for the loop */
    idx_t limit = (idx_t)floor_sqrt((seq_t)m);

    for (idx_t d = 1; d <= limit; ++d)
    {
        if (m % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = m / d;

            if (d1 % 3 == 1)
                sum += (seq_t)d1 + 2;

            if (d2 != d1 && d2 % 3 == 1)
                sum += (seq_t)d2 + 2;
        }
    }

    return sum / 3;
}
