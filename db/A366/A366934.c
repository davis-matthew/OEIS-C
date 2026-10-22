/*
 * A366934  Expansion of Sum_{k>=1} k^5 * x^k/(1 - x^k)^5.
 * https://oeis.org/A366934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^5 * binomial(n/d+3,4)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   divisor_contribution
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A366934 – Expansion of Sum_{k≥1} k^5 * x^k/(1−x^k)^5 */
#include <stddef.h>
#include <stdint.h>
#include "../utils/divisor_contribution.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            seq_t c1 = divisor_contribution(i, n);
            sum += c1;

            idx_t j = n / i;
            if (j != i)
            {
                seq_t c2 = divisor_contribution(j, n);
                sum += c2;
            }
        }
    }
    return sum;
}
