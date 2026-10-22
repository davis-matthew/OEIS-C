/*
 * A364066  Expansion of Sum_{k>0} k * x^k / (1 - x^(3*k-1)).
 * https://oeis.org/A364066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3) * Sum_{d | (3*n-1), d ≡ 2 (mod 3)} (d+1)
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1, n=0 is unused */

    seq_t m = (seq_t)n * (seq_t)3 - (seq_t)1;   /* m = 3*n - 1 */
    seq_t sum = 0;

    /* Enumerate divisors d of m, checking d mod 3 == 2 */
    for (idx_t i = 1; ; ++i)
    {
        seq_t ii = (seq_t)i * (seq_t)i;
        if (ii > m)
            break;

        seq_t d1 = (seq_t)i;
        if (m % d1 == 0)
        {
            seq_t d2 = m / d1;

            if (d1 % 3 == 2)
                sum += d1 + (seq_t)1;

            if (d2 != d1 && d2 % 3 == 2)
                sum += d2 + (seq_t)1;
        }
    }

    return sum / (seq_t)3;
}
