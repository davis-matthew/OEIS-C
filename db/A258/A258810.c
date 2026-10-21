/*
 * A258810  a(n) = n^9 - 1.
 * https://oeis.org/A258810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t n2 = x * x;            /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t n8 = n4 * n4;           /* n^8 */
    seq_t n9 = n8 * x;            /* n^9 */
    return n9 - (seq_t)1;
}
