/*
 * A272162  a(n) = n^5-n+1.
 * https://oeis.org/A272162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 - n + 1
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;
    seq_t n2 = nn * nn;   /* n^2 */
    seq_t n3 = n2 * nn;   /* n^3 */
    seq_t n4 = n3 * nn;   /* n^4 */
    seq_t n5 = n4 * nn;   /* n^5 */
    return n5 - nn + 1;   /* n^5 - n + 1 */
}
