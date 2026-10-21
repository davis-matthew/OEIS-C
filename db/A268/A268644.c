/*
 * A268644  a(n) = 4*n^3 - 3*n^2 - 2*n - 1.
 * https://oeis.org/A268644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*n^3 - 3*n^2 - 2*n - 1
 * Range:     i8 n <= 3, i16 n <= 20, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    /* compute 4*n^3 - 3*n^2 - 2*n - 1 using only seq_t operations */
    seq_t res = 4 * t * t * t - 3 * t * t - 2 * t - (seq_t)1;
    return res;
}
