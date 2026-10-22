/*
 * A400694  a(n) = mu(n^2 + 2).
 * https://oeis.org/A400694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, new
 * Formula:   a(n) = mu(n^2 + 2)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    idx_t m = (idx_t)n * (idx_t)n + 2;
    return mobius(m);
}
