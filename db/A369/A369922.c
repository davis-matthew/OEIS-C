/*
 * A369922  a(n) = 8*n^3 - 6*n - 1.
 * https://oeis.org/A369922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 8*n^3 - 6*n - 1
 * Range:     i8 n <= 2, i16 n <= 16, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* convert index to seq_t */
    seq_t res = (seq_t)8 * N * N * N - (seq_t)6 * N - (seq_t)1;
    return res;
}
