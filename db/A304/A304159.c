/*
 * A304159  a(n) = 2*n^3 - 4*n^2 + 6*n - 2 (n>=1).
 * https://oeis.org/A304159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 - 4*n^2 + 6*n - 2, n >= 1
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t before any arithmetic */
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;          /* n^2 */
    seq_t N3 = N2 * N;         /* n^3 */

    /* Compute the polynomial using only seq_t arithmetic */
    return 2 * N3 - 4 * N2 + 6 * N - 2;
}
