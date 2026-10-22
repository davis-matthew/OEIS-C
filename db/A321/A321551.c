/*
 * A321551  a(n) = Sum_{d|n} (-1)^(d-1)*d^12.
 * https://oeis.org/A321551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∑_{d|n} (-1)^(d-1) * d^12 Computation: iterate over all
 *            positive divisors d of n, accumulate the signed twelfth power of
 *            d. This is O(√n) and works for any n ≥ 1
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 38, i128 n <= 1534,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined but guard against it */
        return 0;

    seq_t result = 0;

    /* iterate over divisors d up to sqrt(n) */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* sign = (-1)^(d-1): +1 for odd d, -1 for even d */
            seq_t sign = (d & 1) ? +1 : -1;
            seq_t term = pow_uint(d, 12);
            result += sign * term;

            idx_t d2 = n / d;
            if (d2 != d)            /* distinct partner divisor */
            {
                seq_t sign2 = (d2 & 1) ? +1 : -1;
                seq_t term2 = pow_uint(d2, 12);
                result += sign2 * term2;
            }
        }
    }

    return result;
}
