/*
 * A380353  a(n) = (n^2 - n + 2) * (5*n^2 - 5*n + 2) / 4.
 * https://oeis.org/A380353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - n + 2) * (5*n^2 - 5*n + 2) / 4
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 242, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term1 = N * N - N + (seq_t)2;
    seq_t term2 = 5 * N * N - 5 * N + (seq_t)2;
    /* Both term1 and term2 are even, so division by 4 is exact. */
    return (term1 / 2) * (term2 / 2);
}
