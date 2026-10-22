/*
 * A389626  One-third of the total number of edges formed after n points have
 *          been placed in general position on each edge of a triangle (as in
 *          A365929).
 * https://oeis.org/A389626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^4/2 - 2*n^3 + 3*n^2/2 + n + 1
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 230, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_t = (seq_t)n;
    seq_t n2 = n_t * n_t;          /* n^2 */
    seq_t n3 = n2 * n_t;           /* n^3 */
    seq_t n4 = n3 * n_t;           /* n^4 */
    seq_t num = 3 * n4 - 4 * n3 + 3 * n2 + 2 * n_t + 2;
    return num / 2;
}
