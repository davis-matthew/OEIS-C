/*
 * A261574  a(n) = n*(n^2 + 3)*(n^6 + 6*n^4 + 9*n^2 + 3).
 * https://oeis.org/A261574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n*(n^2 + 3)*(n^6 + 6*n^4 + 9*n^2 + 3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;             /* n */
    seq_t n2 = N * N;               /* n^2 */
    seq_t term2 = n2 + (seq_t)3;    /* n^2 + 3 */
    seq_t n4 = n2 * n2;             /* n^4 */
    seq_t n6 = n4 * n2;             /* n^6 */
    seq_t G = n6 + (seq_t)6 * n4 + (seq_t)9 * n2 + (seq_t)3;
    return N * term2 * G;
}
