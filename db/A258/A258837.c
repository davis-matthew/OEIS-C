/*
 * A258837  a(n) = 1 - n^2.
 * https://oeis.org/A258837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^2
 * Range:     i8 n <= 11, i16 n <= 181, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return (seq_t)1 - x * x;
}
