/*
 * A332884  a(n) = -n^2 + 21*n - 1.
 * https://oeis.org/A332884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n^2 + 21*n - 1
 * Range:     i8 n <= 25, i16 n <= 191, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return -tn * tn + 21 * tn - 1;
}
