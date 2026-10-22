/*
 * A390922  a(n) is the number of 2 X 2 matrices with elements 1..n where at
 *          least one row and at least one column is strictly increasing.
 * https://oeis.org/A390922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(7*n^3 - n^2 - 2*n)/12
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 292, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t inner = (seq_t)7 * N3 - N2 - (seq_t)2 * N;
    seq_t num   = (N - 1) * inner;
    return num / (seq_t)12;
}
