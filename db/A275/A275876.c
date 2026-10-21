/*
 * A275876  a(n) = 4*n*(n^2 - 3*n - 1)/3.
 * https://oeis.org/A275876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n*(n^2 - 3*n - 1)/3
 * Range:     i8 n <= 4, i16 n <= 21, i32 n <= 921, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;
    seq_t n2   = tn * tn;
    seq_t inner = n2 - (seq_t)3 * tn - (seq_t)1;
    seq_t prod   = (seq_t)4 * tn * inner;
    return prod / (seq_t)3;
}
