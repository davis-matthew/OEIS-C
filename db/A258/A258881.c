/*
 * A258881  a(n) = n + the sum of the squared digits of n.
 * https://oeis.org/A258881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + sum of squared digits of n
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   digit_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_square_sum.h"

seq_t a(idx_t n)
{
    return (seq_t)n + (seq_t)digit_square_sum(n);
}
