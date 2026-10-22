/*
 * A343639  a(n) = (Sum of digits of 9*n) / 9.
 * https://oeis.org/A343639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (Sum of digits of 9*n) / 9
 * Range:     u8 n <= 28, u16 n <= 7480, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Compute 9*n exactly in seq_t
     * Apply digit_sum and divide by 9 to obtain the term. */
    seq_t prod = (seq_t)n * 9;
    seq_t sum  = digit_sum(prod);
    return sum / 9;
}
