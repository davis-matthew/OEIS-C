/*
 * A331700  Binary XOR of squares of divisors of n.
 * https://oeis.org/A331700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = XOR of the squares of the divisors of n
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            seq_t sq1 = (seq_t)d * d;
            result ^= sq1;
            if (d != n / d)
            {
                idx_t d2 = n / d;
                seq_t sq2 = (seq_t)d2 * d2;
                result ^= sq2;
            }
        }
    }
    return result;
}
