/*
 * A364104  Expansion of Sum_{k>0} k * x^k / (1 - x^(5*k-1)).
 * https://oeis.org/A364104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Sum_{d | (5*n-1), d ≡ 4 (mod 5)} (d + 1)
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset 1, only used for safety */

    uint64_t m = 5 * n - 1;   /* m = 5*n - 1  */
    uint64_t sqrt_m = int_sqrt(m);
    seq_t sum = 0;

    for (uint64_t d = 1; d <= sqrt_m; ++d)
    {
        if (m % d == 0)
        {
            /* divisor d itself */
            if (d % 5 == 4)
                sum += (seq_t)d + 1;

            /* paired divisor e = m / d , skip if equal to d */
            uint64_t e = m / d;
            if (e != d && e % 5 == 4)
                sum += (seq_t)e + 1;
        }
    }

    return sum / 5;
}
