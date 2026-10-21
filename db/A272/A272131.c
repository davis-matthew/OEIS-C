/*
 * A272131  a(n) = 384*n^3 - 1184*n^2 + 1228*n - 427.
 * https://oeis.org/A272131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 384*n^3 - 1184*n^2 + 1228*n - 427
 * Range:     i8 none, i16 n <= 5, i32 n <= 178, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 384 * nn * nn * nn - 1184 * nn * nn + 1228 * nn - 427;
}
