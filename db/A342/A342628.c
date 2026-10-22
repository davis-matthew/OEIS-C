/*
 * A342628  a(n) = Sum_{d|n} d^(n-d).
 * https://oeis.org/A342628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n-d)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;
            /* d1^(n-d1) */
            seq_t term1 = big_pow((seq_t)d1, (seq_t)(n - d1));
            sum += term1;

            if (d1 != d2)
            {
                /* d2^(n-d2) */
                seq_t term2 = big_pow((seq_t)d2, (seq_t)(n - d2));
                sum += term2;
            }
        }
    }
    return sum;
}
