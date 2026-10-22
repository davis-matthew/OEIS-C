/*
 * A328667  a(n) = Sum_{d divides n} (-1)^(n + 1 + d + n/d) * d^2.
 * https://oeis.org/A328667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Σ_{d | n} (-1)^(n + 1 + d + n/d) * d^2
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                     /* sequence starts at offset 1 */
        return 0;

    __int128 sum = 0;                /* accumulate with 128–bit precision */

    for (idx_t d = 1; d <= n / d; ++d)   /* iterate divisors up to sqrt(n) */
    {
        if (n % d != 0)
            continue;

        idx_t e = n / d;          /* complementary divisor */

        /* parity of n+1+d+e   (only low bit matters) */
        uint8_t parity = ( (n ^ 1 ^ d ^ e) & 1 );

        __int128 contrib_d = (__int128)d * d;
        __int128 contrib_e = (__int128)e * e;

        if (parity)               /* odd exponent → sign = -1 */
        {
            sum -= contrib_d;
            if (d != e)
                sum -= contrib_e;
        }
        else                      /* even exponent → sign = +1 */
        {
            sum += contrib_d;
            if (d != e)
                sum += contrib_e;
        }
    }

    return (seq_t)sum;            /* final result fits into seq_t by construction */
}
