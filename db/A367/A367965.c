/*
 * A367965  a(n) = (1/8)*(4*n^2 + 6*n + (-1)^n*(2*n*(n + 1) - 1) + 1).
 * https://oeis.org/A367965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8)*(4*n^2 + 6*n + (-1)^n*(2*n*(n + 1) - 1) + 1)
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: for even n = 2k: a(n) = k*(3*k+2); for odd n = 2k-1: a(n) = k*k */
seq_t a(idx_t n) {
    if ((n & 1) == 0) {               /* even index */
        seq_t k = (seq_t)(n >> 1);    /* k = n/2 */
        return k * (3 * k + 2);
    } else {                           /* odd index */
        seq_t k = (seq_t)((n + 1) >> 1); /* k = (n+1)/2 */
        return k * k;
    }
}
