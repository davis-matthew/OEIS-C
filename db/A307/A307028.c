/*
 * A307028  Longest path length in the stacked book graph S_{n+1} square P_n.
 * https://oeis.org/A307028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 1 - 2*floor((n-2)/2)*floor((n-3)/2) for n>1, a(1) =
 *            1
 * Range:     u8 n <= 19, u16 n <= 359, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t N  = (seq_t)n;
    seq_t t1 = (N - 2) / 2;
    seq_t t2 = (N - 3) / 2;
    return N * N + (seq_t)1 - (seq_t)2 * t1 * t2;
}
