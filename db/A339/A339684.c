/*
 * A339684  a(n) = Sum_{d|n} 4^(d-1).
 * https://oeis.org/A339684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 4^(d-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            seq_t term1 = pow4(i - 1);
            sum += term1;
            idx_t d2 = n / i;
            if (d2 != i)
            {
                seq_t term2 = pow4(d2 - 1);
                sum += term2;
            }
        }
    }
    return sum;
}
