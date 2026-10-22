/*
 * A377663  a(n) = 2*n^3 - 3*n + 1.
 * https://oeis.org/A377663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 - 3*n + 1
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return 2 * N * N * N - 3 * N + 1;
}
