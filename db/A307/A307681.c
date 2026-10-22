/*
 * A307681  Difference between the number of diagonals and the number of sides
 *          for a convex n-gon.
 * https://oeis.org/A307681
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(n-5)/2
 * Range:     i8 n <= 18, i16 n <= 258, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return m * (m - 5) / (seq_t)2;
}
