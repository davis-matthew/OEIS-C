/*
 * A263229  a(n) = 4*n*(21*n - 26).
 * https://oeis.org/A263229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n*(21*n - 26)
 * Range:     i8 n <= 1, i16 n <= 20, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 4 * nn * (21 * nn - 26);
}
