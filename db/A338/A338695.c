/*
 * A338695  a(n) = Sum_{d|n} 2^(d-1) * binomial(d, n/d).
 * https://oeis.org/A338695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(d-1) * binomial(d, n/d)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* term for divisor d1 */
            seq_t term = binomial(d1, n / d1);
            term = term * pow2(d1 - 1);
            result += term;

            /* term for divisor d2 if different */
            if (d1 != d2)
            {
                seq_t term2 = binomial(d2, n / d2);
                term2 = term2 * pow2(d2 - 1);
                result += term2;
            }
        }
    }
    return result;
}
