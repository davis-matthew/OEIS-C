/*
 * A267472  Number of length-4 0..n arrays with no following elements larger
 *          than the first repeated value.
 * https://oeis.org/A267472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + (17/6)*n^3 + 4*n^2 + (19/6)*n + 1 (equivalently:
 *            a(n) = (6*n^4 + 17*n^3 + 24*n^2 + 19*n + 6) / 6)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 174, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;     /* cast to seq_t once */
    seq_t n2 = N * N;        /* n^2   */
    seq_t n3 = n2 * N;       /* n^3   */
    seq_t n4 = n3 * N;       /* n^4   */
    seq_t num = 6 * n4 + 17 * n3 + 24 * n2 + 19 * N + 6;
    return num / 6;
}
