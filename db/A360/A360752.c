/*
 * A360752  Expansion of Sum_{k>0} (x * (1 + (2 * x)^k))^k.
 * https://oeis.org/A360752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(n-d) * binomial(d, n/d-1)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 34, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binomial, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* divisor d1 */
            seq_t term1 = seq_mul(pow2(n - d1), binomial(d1, n / d1 - 1));
            sum += term1;

            /* divisor d2, if distinct */
            if (d1 != d2)
            {
                seq_t term2 = seq_mul(pow2(n - d2), binomial(d2, n / d2 - 1));
                sum += term2;
            }
        }
    }
    return sum;
}
