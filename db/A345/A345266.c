/*
 * A345266  a(n) = Sum_{p|n, p prime} gcd(p,n/p).
 * https://oeis.org/A345266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{p|n, p prime} gcd(p, n/p)
 * Range:     u8 n <= 9002, u16 n <= 9002, u32 n <= 9002, u64 n <= 9002,
 *            u128 n <= 9002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence. */
    if (n < 2) return 0;

    uint64_t m = (uint64_t)n;   /* online copy to factor */
    seq_t sum = 0;

    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            unsigned e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            sum += (seq_t)((e >= 2) ? p : 1);
        }
    }

    /* Any remaining factor must be a prime with exponent 1. */
    if (m > 1)
        sum += 1;

    return sum;
}
