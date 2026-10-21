/*
 * A268264  Number of length-(5+1) 0..n arrays with new repeated values
 *          introduced in sequential order starting with zero.
 * https://oeis.org/A268264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + n^5 + 5*n^4 + 4*n^3 + 12*n^2 + 6*n + 5 for n > 1
 *            a(1) = 33
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, but safe fallback */
    if (n == 1) return (seq_t)33;          /* first term of the sequence        */

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;                      /* n^2 */
    seq_t N3 = N2 * N;                     /* n^3 */
    seq_t N4 = N3 * N;                     /* n^4 */
    seq_t N5 = N4 * N;                     /* n^5 */
    seq_t N6 = N5 * N;                     /* n^6 */

    return N6 + N5 + 5 * N4 + 4 * N3 + 12 * N2 + 6 * N + 5;
}
