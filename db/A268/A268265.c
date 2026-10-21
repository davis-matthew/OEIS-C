/*
 * A268265  Number of length-(6+1) 0..n arrays with new repeated values
 *          introduced in sequential order starting with zero.
 * https://oeis.org/A268265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + n^6 + 6*n^5 + 5*n^4 + 20*n^3 + 12*n^2 + 20*n + 5
 *            (n>1); a(1) = 65
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)65;

    seq_t nn  = (seq_t)n;
    seq_t nn2 = nn * nn;           /* n^2   */
    seq_t nn3 = nn2 * nn;          /* n^3   */
    seq_t nn4 = nn3 * nn;          /* n^4   */
    seq_t nn5 = nn4 * nn;          /* n^5   */
    seq_t nn6 = nn5 * nn;          /* n^6   */
    seq_t nn7 = nn6 * nn;          /* n^7   */

    seq_t res = nn7
             + nn6
             + (seq_t)6 * nn5
             + (seq_t)5 * nn4
             + (seq_t)20 * nn3
             + (seq_t)12 * nn2
             + (seq_t)20 * nn
             + (seq_t)5;

    return res;
}
