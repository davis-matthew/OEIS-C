/*
 * A367122  Place n points in general position on each side of a square, and
 *          join every pair of the 4*n+4 boundary points by a chord; sequence
 *          gives number of edges in the resulting planar graph.
 * https://oeis.org/A367122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*n^4 + 38*n^3 + 37*n^2 + 24*n + 8
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t n4  = n3 * N;
    return 17 * n4 + 38 * n3 + 37 * n2 + 24 * N + 8;
}
