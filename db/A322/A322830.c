/*
 * A322830  a(n) = 32*n^3 + 48*n^2 + 18*n + 1.
 * https://oeis.org/A322830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^3 + 48*n^2 + 18*n + 1
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;      /* n^2 */
    seq_t n3 = n2 * N;     /* n^3 */
    return 32 * n3 + 48 * n2 + 18 * N + 1;
}
