/*
 * A268458  Number of length-4 0..n arrays with no adjacent pair x,x+1
 *          followed at any distance by x+1,x.
 * https://oeis.org/A268458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 + 4*n^2 + n + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n < 1 we return 0. */
    if (n < 1)
        return 0;

    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;           /* n^2 */
    seq_t n3 = n2 * tn;           /* n^3 */
    seq_t n4 = n3 * tn;           /* n^4 */

    return n4 + (seq_t)4 * n3 + (seq_t)4 * n2 + tn + 1;
}
