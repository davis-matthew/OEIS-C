/*
 * A330700  a(n) = (n - 1)*n*(2*n^2 + 4*n - 1)/6.
 * https://oeis.org/A330700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n - 1)*n*(2*n^2 + 4*n - 1)/6
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2u) return 0;
    seq_t N = (seq_t)n;
    seq_t term1 = N * (N - 1);
    seq_t term2 = 2 * N * N + 4 * N - 1;
    return term1 * term2 / 6;
}
