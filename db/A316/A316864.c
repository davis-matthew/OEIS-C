/*
 * A316864  Number of times 3 appears in decimal expansion of n.
 * https://oeis.org/A316864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of times digit 3 appears in the decimal expansion
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count the '3' digits by repeated division by 10. */
    idx_t count = 0;
    while (n > 0)
    {
        if (n % 10 == 3)
            ++count;
        n /= 10;
    }
    return (seq_t)count;
}
