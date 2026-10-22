/*
 * A302576  Numbers k such that k/10 + 1 is a square.
 * https://oeis.org/A302576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 10*n*(n-2)
 * Range:     i8 n <= 4, i16 n <= 58, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn - 2) * 10;
}
