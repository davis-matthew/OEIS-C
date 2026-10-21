/*
 * A248974  Floor( 1/(n*sinh(1/n) + n*sin(1/n) - 2) ).
 * https://oeis.org/A248974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*n^4 - 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s = (seq_t)n;          /* convert index to seq_t */
    seq_t s2 = s * s;            /* n^2 */
    seq_t s4 = s2 * s2;          /* n^4 */
    seq_t res = s4 * 60 - 1;     /* 60*n^4 - 1 */
    return res;
}
