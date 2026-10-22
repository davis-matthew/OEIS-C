/*
 * A330651  a(n) = n^4 + 3*n^3 + 2*n^2 - 2*n.
 * https://oeis.org/A330651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 3*n^3 + 2*n^2 - 2*n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;    /* n^2 */
    seq_t x3 = x2 * x;   /* n^3 */
    seq_t x4 = x3 * x;   /* n^4 */
    return x4 + 3 * x3 + 2 * x2 - 2 * x;
}
