/*
 * A242643  a(n) = Sum_{d|n, d <= 6} d^2 + 6*Sum_{d|n, d>6} d.
 * https://oeis.org/A242643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d <= 6} d^2 + 6*Sum_{d|n, d>6} d
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;
            if (d <= 6)
                result += (seq_t)d * (seq_t)d;
            else
                result += (seq_t)6 * (seq_t)d;

            if (e != d)
            {
                if (e <= 6)
                    result += (seq_t)e * (seq_t)e;
                else
                    result += (seq_t)6 * (seq_t)e;
            }
        }
    }
    return result;
}
