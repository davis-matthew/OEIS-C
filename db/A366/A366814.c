/*
 * A366814  a(n) = Sum_{d|n} (-1)^(n/d-1) * binomial(d+3,4).
 * https://oeis.org/A366814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d-1) * binomial(d+3,4)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 405, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_d_plus3_choose4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_d_plus3_choose4.h"

seq_t a(idx_t n)
{
    long long sum = 0;       /* signed accumulator for intermediate sums */
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i != 0)
            continue;
        idx_t d1 = i;
        idx_t d2 = n / i;   /* d2 is the paired divisor */

        /* contribution from divisor d1 */
        seq_t t1 = binom_d_plus3_choose4(d1);
        if ((d2 & 1) == 0)   /* n/d1 = d2 even -> negative sign */
            sum -= (long long)t1;
        else
            sum += (long long)t1;

        if (d1 != d2) /* avoid double counting square root divisor */
        {
            seq_t t2 = binom_d_plus3_choose4(d2);
            if ((d1 & 1) == 0)   /* n/d2 = d1 even -> negative sign */
                sum -= (long long)t2;
            else
                sum += (long long)t2;
        }
    }
    return (seq_t)sum;
}
