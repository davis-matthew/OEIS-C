/*
 * A363706  a(n) is the sigma irregularity of the n-th power of a path graph
 *          of length at least 3*n.
 * https://oeis.org/A363706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 2*n^3 + 2*n^2 + n)/3
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    return (N*N*N*N + 2*N*N*N + 2*N*N + N) / 3;
}
