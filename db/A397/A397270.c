/*
 * A397270  a(n) = (2*n-3-(-1)^n)*(32*n^2-9*n+7*n*(-1)^n)/96.
 * https://oeis.org/A397270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-3-(-1)^n)*(32*n^2-9*n+7*n*(-1)^n)/96 Equivalently:
 *            a(2*k) = k*(k-1)*(32*k-1)/6 a(2*k+1) = 2*k*(2*k+1)*(4*k+1)/3
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    if ((n & 1ULL) == 0) {                       /* n is even: n = 2*k */
        seq_t k = N >> 1;                        /* k = n/2   */
        return k * (k - 1) * (32 * k - 1) / 6;
    } else {                                     /* n is odd: n = 2*k+1 */
        seq_t k = (N - 1) >> 1;                  /* k = (n-1)/2 */
        return 2 * k * (2 * k + 1) * (4 * k + 1) / 3;
    }
}
