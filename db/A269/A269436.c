/*
 * A269436  Number of length-4 0..n arrays with no repeated value greater than
 *          the previous repeated value.
 * https://oeis.org/A269436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 + (11*n^2 + 7*n)/2 + 1 (OEIS A269436)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the offset a(1) being the first term.  For n = 0 the
       sequence is undefined; return 0 in that case. */
    if (n == 0) return (seq_t)0;

    seq_t tn  = (seq_t)n;          /* n */
    seq_t n2 = tn * tn;            /* n^2 */
    seq_t n3 = n2 * tn;            /* n^3 */
    seq_t n4 = n3 * tn;            /* n^4 */

    /* Combine the fractional part (11*n^2 + 7*n) / 2.
       The numerator is always even, so the division truncates to the
       correct integer value. */
    seq_t temp = (seq_t)11 * n2 + (seq_t)7 * tn;
    seq_t add  = temp / (seq_t)2;

    /* Final polynomial evaluation. */
    seq_t result = n4 + (seq_t)4 * n3 + add + (seq_t)1;
    return result;
}
