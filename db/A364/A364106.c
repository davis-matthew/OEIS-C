/*
 * A364106  Expansion of Sum_{k>0} k * x^(3*k) / (1 - x^(5*k-1)).
 * https://oeis.org/A364106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Σ_{d | 5*n-3 , d ≡ 4 (mod 5)} (d+1)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n ≥ 1 */

    idx_t T = 5*n - 3;
    seq_t sum = 0;

    for (idx_t i = 1; i*i <= T; ++i)
    {
        if (T % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = T / i;

            if (d1 % 5 == 4)
                sum += (seq_t)(d1 + 1);

            if (d2 != d1 && d2 % 5 == 4)
                sum += (seq_t)(d2 + 1);
        }
    }

    return sum / 5;   /* division is exact by construction */
}
