/*
 * A276351  a(n) = 2*(3 + 2 n + 3 n^2 + 3 n^3 + 3 n^4 + n^5 + n^6).
 * https://oeis.org/A276351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(3 + 2*n + 3*n^2 + 3*n^3 + 3*n^4 + n^5 + n^6)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;       /* convert index to seq_t for arithmetic */
    seq_t res = 2;            /* coefficient of n^6 */
    res = res * x + 2;        /* add coefficient of n^5 */
    res = res * x + 6;        /* add coefficient of n^4 */
    res = res * x + 6;        /* add coefficient of n^3 */
    res = res * x + 6;        /* add coefficient of n^2 */
    res = res * x + 4;        /* add coefficient of n   */
    res = res * x + 6;        /* add constant term       */
    return res;
}
