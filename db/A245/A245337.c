/*
 * A245337  Sum of digits of n in fractional base 7/6.
 * https://oeis.org/A245337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of digits of n in base 7/6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n;

    while (x != 0) {
        idx_t d = x % (idx_t)7;           /* next digit (0..6)            */
        sum += (seq_t)d;                  /* accumulate the digit sum     */
        x = ((x - d) / (idx_t)7) * (idx_t)6; /* remove this digit and scale */
    }

    return sum;
}
