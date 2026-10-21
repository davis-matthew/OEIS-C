/*
 * A277982  a(n) = 12*n^2 + 10*n - 30.
 * https://oeis.org/A277982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12*n^2 + 10*n - 30
 * Range:     i8 n <= 3, i16 n <= 40, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 12 * nn * nn + 10 * nn - 30;
    return res;
}
