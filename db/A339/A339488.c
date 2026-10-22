/*
 * A339488  a(n) = H(n-1, n, n+1) where H(a, b, c) = (a + b + c)*(a + b -
 *          c)*(b + c - a)*(c + a - b) is Heron's polynomial.
 * https://oeis.org/A339488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*n^4 - 12*n^2
 * Range:     i8 n <= 2, i16 n <= 10, i32 n <= 163, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t nnnn = nn * nn;                   /* n^4 */
    return 3 * nnnn - 12 * nn;
}
