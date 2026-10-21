/*
 * A270870  a(n) = n^6 + 5*n^5 + 19*n^4 + 44*n^3 + 72*n^2 + 69*n + 5.
 * https://oeis.org/A270870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 + 5*n^5 + 19*n^4 + 44*n^3 + 72*n^2 + 69*n + 5
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t res = 1;          /* coefficient of n^6 */
    res = res * N + 5;      /* n^5 coefficient */
    res = res * N + 19;     /* n^4 coefficient */
    res = res * N + 44;     /* n^3 coefficient */
    res = res * N + 72;     /* n^2 coefficient */
    res = res * N + 69;     /* n coefficient */
    res = res * N + 5;      /* constant term */
    return res;
}
