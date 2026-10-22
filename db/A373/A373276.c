/*
 * A373276  a(n) = Sum_{d|n} (-1)^(d-1) * 3^(n/d-1).
 * https://oeis.org/A373276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * 3^(n/d-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for indices below offset */
    seq_t sum = 0;

    /* compute floor(sqrt(n)) */
    idx_t sqrt_n = 0;
    while ((sqrt_n + 1) * (sqrt_n + 1) <= n) sqrt_n++;

    for (idx_t d = 1; d <= sqrt_n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            seq_t term1 = pow3(n / d - 1);
            if (d & 1U)   sum += term1;
            else          sum -= term1;

            /* paired divisor d2 = n/d, skip if equal to d */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t term2 = pow3(d - 1);   /* since n/d2 = d */
                if (d2 & 1U)   sum += term2;
                else          sum -= term2;
            }
        }
    }
    return sum;
}
