/*
 * A269644  Number of length-7 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus two or minus
 *          1.
 * https://oeis.org/A269644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 6*n^5 + 20*n^4 - 22*n^3 + 28*n^2 - 37*n +
 *            13 for n>2 a(1) = 20, a(2) = 969
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)969;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;         /* n^2 */
    seq_t n3 = n2 * nn;         /* n^3 */
    seq_t n4 = n3 * nn;         /* n^4 */
    seq_t n5 = n4 * nn;         /* n^5 */
    seq_t n6 = n5 * nn;         /* n^6 */
    seq_t n7 = n6 * nn;         /* n^7 */

    seq_t res = n7
              + (seq_t)7 * n6
              + (seq_t)6 * n5
              + (seq_t)20 * n4
              - (seq_t)22 * n3
              + (seq_t)28 * n2
              - (seq_t)37 * nn
              + (seq_t)13;
    return res;
}
