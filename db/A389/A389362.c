/*
 * A389362  Number of divisors d of n such that d^2 - 1 is not relatively
 *          prime to n.
 * https://oeis.org/A389362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n) - A386476(n)
 * Range:     u8 n <= 23, u16 n <= 637, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of divisors d of n such that d^2 – 1 is not relatively prime to n */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    seq_t count = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            seq_t dd = (seq_t)d;
            dd = dd * dd - 1;
            if (gcd(dd, (seq_t)n) > 1)
                ++count;

            /* complementary divisor n/d, avoid double counting when d == n/d */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t dd2 = (seq_t)d2;
                dd2 = dd2 * dd2 - 1;
                if (gcd(dd2, (seq_t)n) > 1)
                    ++count;
            }
        }
    }

    return count;
}
