/*
 * A318765  a(n) = (n + 2)*(n^2 + n - 1).
 * https://oeis.org/A318765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n+2)*(n^2 + n - 1)
 * Range:     i8 n <= 4, i16 n <= 31, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t inner = nseq * nseq + nseq - (seq_t)1;
    return (nseq + (seq_t)2) * inner;
}
