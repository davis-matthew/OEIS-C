/*
 * A269681  Number of length-6 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus or minus one
 *          modulo n+1.
 * https://oeis.org/A269681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n^6 + 6*n^5 + 5*n^4 + 12*n^3 + 6*n^2 + 6   for n > 1
 *           a(1) = 22
 */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)22;

    seq_t nn = (seq_t)n;
    seq_t p2 = nn * nn;        /* n^2   */
    seq_t p3 = p2 * nn;        /* n^3   */
    seq_t p4 = p3 * nn;        /* n^4   */
    seq_t p5 = p4 * nn;        /* n^5   */
    seq_t p6 = p5 * nn;        /* n^6   */

    seq_t result = p6
                 + (seq_t)6 * p5
                 + (seq_t)5 * p4
                 + (seq_t)12 * p3
                 + (seq_t)6 * p2
                 + (seq_t)6;

    return result;
}
