/*
 * A400695  a(n) = mu(n^2 + 3).
 * https://oeis.org/A400695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, new
 * Formula:   a(n) = mu(n^2 + 3)
 * Range:     i8 n <= 11, i16 n <= 1595, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Compute n^2 + 3 using seq_t to avoid overflow */
    seq_t m = (seq_t)n * (seq_t)n + 3;
    /* Return Möbius value of the computed number */
    return mobius((idx_t)m);
}
