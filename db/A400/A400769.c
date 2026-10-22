/*
 * A400769  a(n) = mu(n^2 + n + 1).
 * https://oeis.org/A400769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, new
 * Formula:   a(n) = mu(n^2 + n + 1)
 * Range:     i8 n <= 11, i16 n <= 368, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Compute n^2 + n + 1 using seq_t arithmetic. */
    seq_t val = (seq_t)n * ((seq_t)n + 1) + 1;
    /* Pass the value to the helper; for the ranges used it fits into idx_t. */
    return mobius((idx_t)val);
}
