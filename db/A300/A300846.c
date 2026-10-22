/*
 * A300846  a(n) = 3*(n - 1)^2*n^3.
 * https://oeis.org/A300846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n - 1)^2*n^3
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn * nn;          /* n^3 */
    seq_t tmp = nn - 1;
    tmp = tmp * tmp;                   /* (n-1)^2 */
    res = res * tmp;                   /* n^3*(n-1)^2 */
    res = res * 3;                     /* times 3 */
    return res;
}
