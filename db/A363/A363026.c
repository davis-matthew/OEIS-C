/*
 * A363026  Sum of divisors of 5*n-3 of form 5*k+2.
 * https://oeis.org/A363026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d | 5n-3, d ≡ 2 (mod 5)} d
 * Range:     u8 n <= 44, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n == 0 we return 0. */
    if (n == 0)
        return 0;

    /* Compute m = 5*n - 3.  For the intended range of n this fits in uint64_t. */
    uint64_t m = 5 * n - 3;

    seq_t sum = 0;

    /* Enumerate all divisors d of m up to sqrt(m).
       For each divisor check if it is ≡ 2 (mod 5), and also check the paired divisor.  */
    for (uint64_t d = 1; d <= m / d; ++d)
    {
        if (m % d == 0)
        {
            if (d % 5 == 2)
                sum += (seq_t)d;  /* cast so that addition uses seq_t arithmetic */

            uint64_t e = m / d;
            if (e != d && e % 5 == 2)
                sum += (seq_t)e;
        }
    }

    return sum;
}
