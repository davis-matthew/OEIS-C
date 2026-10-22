/*
 * A389882  Row sums of A389420 and A389875.
 * https://oeis.org/A389882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = (sum of decimal digits of n) * ((10^len(n) - 1) / 9) i.e.
 *            a(n) = digit_sum(n) * repunit_of_len(len(n))
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  seq_t and idx_t are defined by the build system.            */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t  t      = n;
    seq_t  sum    = (seq_t)0;
    int    digits = 0;

    while (t > 0) {
        sum    += (seq_t)(t % 10);
        t      /= 10;
        digits += 1;
    }

    seq_t repunit = (seq_t)1;
    for (int i = 1; i < digits; ++i)
        repunit = repunit * 10 + 1;

    return sum * repunit;
}
