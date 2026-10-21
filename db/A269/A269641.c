/*
 * A269641  Number of length-4 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus two or minus
 *          1.
 * https://oeis.org/A269641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 + 3*n^2 + 2*n - 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast to seq_t for correct arithmetic */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */
    seq_t n4 = n3 * sn;           /* n^4 */
    return n4 + (seq_t)4 * n3 + (seq_t)3 * n2 + (seq_t)2 * sn - (seq_t)1;
}
