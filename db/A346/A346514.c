/*
 * A346514  a(n) = n^4 + 28*n^3 + 252*n^2 + 784*n + 448.
 * https://oeis.org/A346514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 28*n^3 + 252*n^2 + 784*n + 448
 * Range:     u8 none, u16 n <= 9, u32 n <= 249, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* n */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */
    seq_t res = n4
              + (seq_t)28 * n3
              + (seq_t)252 * n2
              + (seq_t)784 * nn
              + (seq_t)448;
    return res;
}
