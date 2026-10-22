/*
 * A337843  a(n) is n + the number of digits in the decimal expansion of n.
 * https://oeis.org/A337843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + digits(n)
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t temp = n;
    seq_t digits = 0;

    if (temp == 0)
        digits = 1;
    else
    {
        while (temp > 0)
        {
            digits++;
            temp /= 10;
        }
    }

    return (seq_t)n + digits;
}
