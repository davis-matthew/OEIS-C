/*
 * A268228  a(n) = sum of digits of (2n + 1)^2.
 * https://oeis.org/A268228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of (2n+1)^2
 * Range:     u8 n <= 7, u16 n <= 184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* compute x = 2*n + 1 as seq_t */
    seq_t x = (seq_t)n * (seq_t)2 + (seq_t)1;

    /* square */
    seq_t sq = x * x;

    /* return sum of decimal digits */
    return digit_sum(sq);
}
