/*
 * A245627  Base 10 digit sum of 11*n.
 * https://oeis.org/A245627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digit_sum(11*n)
 * Range:     u8 n <= 23, u16 n <= 6425, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* compute 11*n, then sum of its decimal digits */
    seq_t prod = ((seq_t)n) * 11;
    return digit_sum(prod);
}
