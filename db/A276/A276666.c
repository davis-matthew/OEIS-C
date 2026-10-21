/*
 * A276666  a(n) = (n-1)*Catalan(n).
 * https://oeis.org/A276666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-1)*Catalan(n)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 33, i128 n <= 65,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Catalan(0) = 1, so a(0) = -1, and a(1) = 0 automatically. */
    return ((seq_t)n - 1) * catalan(n);
}
