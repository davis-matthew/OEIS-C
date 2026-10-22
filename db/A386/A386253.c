/*
 * A386253  a(n) = (smallest digit of n)^(largest digit of n) + n.
 * https://oeis.org/A386253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (smallest digit of n)^(largest digit of n) + n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* by convention 0^0 = 1 */

    idx_t x = n;
    idx_t min = 9, max = 0;

    /* find smallest and largest decimal digit */
    while (x > 0) {
        idx_t d = x % 10;
        if (d < min) min = d;
        if (d > max) max = d;
        x /= 10;
    }

    /* compute min^max */
    seq_t pow = 1;
    for (idx_t i = 0; i < max; ++i)
        pow *= (seq_t)min;

    return (seq_t)n + pow;
}
