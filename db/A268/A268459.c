/*
 * A268459  Number of length-5 0..n arrays with no adjacent pair x,x+1
 *          followed at any distance by x+1,x.
 * https://oeis.org/A268459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + 7*n^3 + n^2 + 2*n
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n;          /* n */
    seq_t n2 = n1 * n1;           /* n^2 */
    seq_t n3 = n2 * n1;           /* n^3 */
    seq_t n4 = n3 * n1;           /* n^4 */
    seq_t n5 = n4 * n1;           /* n^5 */

    seq_t res = n5 + 5 * n4 + 7 * n3 + n2 + 2 * n1;
    return res;
}
