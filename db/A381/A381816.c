/*
 * A381816  a(n) = (4*n^2 - (-1)^n - 2*n - 7)/4.
 * https://oeis.org/A381816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (4*n^2 - (-1)^n - 2*n - 7)/4
 * Range:     i8 n <= 6, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term = (seq_t)4 * nseq * nseq
                - (n & 1 ? (seq_t)-1 : (seq_t)1)
                - (seq_t)2 * nseq
                - (seq_t)7;
    return term / 4;
}
