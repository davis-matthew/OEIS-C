/*
 * A321180  a(n) = 17*n^2 - 1.
 * https://oeis.org/A321180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = 17*n^2 - 1
 * Range:     i8 n <= 2, i16 n <= 43, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return m * m * 17 - 1;
}
