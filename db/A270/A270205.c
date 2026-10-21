/*
 * A270205  Number of 2 X 2 planar subsets in an n X n X n cube.
 * https://oeis.org/A270205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^3 - 6*n^2 + 3*n
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    return (seq_t)3 * n3 - (seq_t)6 * n2 + (seq_t)3 * N;
}
