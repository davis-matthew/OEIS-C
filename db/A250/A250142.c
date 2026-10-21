/*
 * A250142  Number of length 3+2 0..n arrays with the medians of every three
 *          consecutive terms nondecreasing.
 * https://oeis.org/A250142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (13*n^5 + 90*n^4 + 220*n^3 + 240*n^2 + 127*n + 30) / 30
 *            (Valid for all n ≥ 1; a(0) = 0)
 * Range:     u8 none, u16 n <= 4, u32 n <= 71, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Promote index to seq_t for arithmetic. */
    seq_t nn = (seq_t)n;

    /* Compute successive powers of n. */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */

    /* Build the numerator of the rational expression. */
    seq_t num = (seq_t)13 * n5
              + (seq_t)90 * n4
              + (seq_t)220 * n3
              + (seq_t)240 * n2
              + (seq_t)127 * nn
              + (seq_t)30;

    /* Divide by 30 (exact division for all integer n). */
    return num / (seq_t)30;
}
