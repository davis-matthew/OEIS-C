/*
 * A269620  Number of length-4 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus two, zero or
 *          minus 1.
 * https://oeis.org/A269620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 + 5*n^2 + 5*n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;      /* cast index to seq_t */
    seq_t N2 = N * N;         /* n^2 */
    seq_t N3 = N2 * N;        /* n^3 */
    seq_t N4 = N3 * N;        /* n^4 */
    return N4 + 4 * N3 + 5 * N2 + 5 * N;
}
