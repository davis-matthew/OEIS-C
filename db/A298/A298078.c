/*
 * A298078  a(n) = 7*n^2 - 7*n - 43.
 * https://oeis.org/A298078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7*n^2 - 7*n - 43
 * Range:     i8 n <= 5, i16 n <= 68, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 7 * tn * tn - 7 * tn - 43;
}
