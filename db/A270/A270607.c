/*
 * A270607  Number of 2 X 2 X 2 triangular 0..n arrays with some element plus
 *          some adjacent element totaling n+1 or n-1 exactly once.
 * https://oeis.org/A270607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 6; for n > 1, a(n) = 6*n^2 - 18*n + 12 if n is even,
 *            6*n^2 - 18*n + 36 if n is odd
 * Range:     u8 n <= 8, u16 n <= 106, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)6;
    seq_t N = (seq_t)n;
    if ((n & 1) == 0)          /* even */
        return 6 * N * N - 18 * N + 12;
    else                       /* odd */
        return 6 * N * N - 18 * N + 36;
}
