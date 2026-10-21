/*
 * A269437  Number of length-5 0..n arrays with no repeated value greater than
 *          the previous repeated value.
 * https://oeis.org/A269437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + (17/2)*n^3 + 8*n^2 + (9/2)*n + 1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                         /* offset starts at 1 */
    seq_t tn = (seq_t)n;

    seq_t n2 = tn * tn;                         /* n^2   */
    seq_t n3 = n2 * tn;                         /* n^3   */
    seq_t n4 = n3 * tn;                         /* n^4   */
    seq_t n5 = n4 * tn;                         /* n^5   */

    /* (17*n^3 + 9*n) is always even, so division is exact */
    seq_t rational = (17 * n3 + 9 * tn) / 2;

    return n5 + (5 * n4) + (8 * n2) + rational + 1;
}
