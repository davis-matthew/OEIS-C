/*
 * A281381  a(n) = n*(n + 1)*(4*n + 5)/2.
 * https://oeis.org/A281381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(4*n+5)/2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = n;              /* cast idx_t to seq_t */
    seq_t t = 4*m + 5;        /* 4*n + 5   */
    return (m * (m + 1) * t) / 2;
}
