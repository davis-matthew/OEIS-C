/*
 * A280637  Sum of the digits of n^2+1.
 * https://oeis.org/A280637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of decimal digits of (n^2 + 1)
 * Range:     u8 n <= 15, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following header provides the digit_sum routine. */
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Compute n^2 + 1 in seq_t arithmetic. */
    seq_t sq = (seq_t)n * (seq_t)n + (seq_t)1;
    /* Return the sum of its decimal digits. */
    return digit_sum(sq);
}
