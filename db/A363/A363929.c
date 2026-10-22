/*
 * A363929  Expansion of Sum_{k>0} x^(4*k) / (1 - x^(5*k))^2.
 * https://oeis.org/A363929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Sum_{d|n, d≡4 (mod 5)} (d+1)
 * Range:     u8 n <= 1103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;

            if (d1 % 5 == 4)
                total += (seq_t)d1 + 1;

            if (d2 != d1 && d2 % 5 == 4)
                total += (seq_t)d2 + 1;
        }
    }
    return total / 5;
}
