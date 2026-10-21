/*
 * A249354  a(n) = n*(3*n^2 + 3*n + 1).
 * https://oeis.org/A249354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n^2 + 3*n + 1)
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_t = (seq_t)n;            /* convert index to seq_t */
    seq_t nn_sq = n_t * n_t;         /* n^2 */
    seq_t part = (seq_t)3 * nn_sq + (seq_t)3 * n_t + (seq_t)1; /* 3*n^2 + 3*n + 1 */
    return n_t * part;               /* n * (3*n^2 + 3*n + 1) */
}
