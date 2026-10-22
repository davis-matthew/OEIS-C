/*
 * A357779  Maximum number of edges in a 6-degenerate graph with n vertices.
 * https://oeis.org/A357779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2) for n < 8; 6*n-21 for n >= 8
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n < 8) {
        seq_t nn = (seq_t)n;
        return nn * (nn - 1) / 2;
    }
    return (seq_t)n * 6 - 21;
}
