/*
 * A268227  a(n) = sum of digits of (2n)^2.
 * https://oeis.org/A268227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of decimal digits of (2*n)^2
 * Range:     u8 n <= 7, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 4 * nn * nn;
    seq_t sum = 0;

    while (val > 0) {
        sum += val % 10;
        val /= 10;
    }

    return sum;
}
