/*
 * A346388  a(n) is the number of proper divisors of A053742(n) ending with 5.
 * https://oeis.org/A346388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tau((25+50n)/5) - 1 = tau(10n+5) - 1, since every divisor
 *            of the odd number A053742(n) = 25+50n that ends in 5 is a
 *            multiple of 5
 * Range:     u8 n <= 25, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 84 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)10 * (seq_t)n + 5;
    seq_t tau = 1;
    for (seq_t p = 3; p * p <= m; p += 2)
    {
        if (m % p == 0)
        {
            seq_t e = 1;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            tau *= e;
        }
    }
    if (m > 1)
        tau *= 2;
    return tau - 1;
}
