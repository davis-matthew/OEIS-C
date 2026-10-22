/*
 * A359738  a(n) = [x^n] (2*x^4 + 2*x^3 + 2*x^2 + x + 1)/(x^2 + 1).
 * https://oeis.org/A359738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for n < 3, a(n) = (-1)^((n-3)/2) for n ≥ 3 (i.e., 1 if
 *            (n-3)/2 is even, -1 otherwise)
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)1;
    idx_t exp = (n - 3) / 2;
    return (seq_t)((exp & 1) ? -1 : 1);
}
