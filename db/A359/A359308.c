/*
 * A359308  Number of divisors of 6*n-4 of form 6*k+1.
 * https://oeis.org/A359308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of divisors of (6*n-4) that are congruent to 1 modulo 6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* m = 6*n - 4 is always positive for n >= 1 */
    idx_t m = 6*n - 4;

    seq_t cnt = 0;
    /* iterate over all possible divisor candidates up to sqrt(m) */
    for (idx_t d = 1; d*d <= m; ++d)
    {
        if (m % d == 0)
        {
            /* divisor d */
            if (d % 6 == 1)
                ++cnt;
            /* paired divisor m/d, avoid double counting when m is a perfect square */
            idx_t e = m / d;
            if (e != d && e % 6 == 1)
                ++cnt;
        }
    }
    return cnt;
}
