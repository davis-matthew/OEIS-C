/*
 * A283551  a(n) = -1 + 5*n/6 + n^3/6.
 * https://oeis.org/A283551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -1 + n*(n^2+5)/6
 * Range:     i8 n <= 8, i16 n <= 58, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t val = N * (N * N + 5) / 6 - 1;
    return val;
}
