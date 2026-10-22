/*
 * A376922  Variance of n-th power of a standard normal random variable.
 * https://oeis.org/A376922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = M(2n) - M(n)^2 (M(k) = 0 if k is odd, else (k-1)!!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute M(2n) = double factorial of (2n-1) */
    idx_t m2n = 2 * n;
    seq_t M2n = 1;
    for (idx_t i = 3; i <= m2n; i += 2) {
        M2n *= (seq_t)i;
    }

    /* Compute M(n) only if n is even; else it is 0 */
    seq_t Mn = 0;
    if ((n & 1) == 0) {                 /* n even */
        Mn = 1;
        for (idx_t i = 3; i <= n; i += 2) {
            Mn *= (seq_t)i;
        }
    }

    /* a(n) = M2n - Mn^2  (Mn^2 is zero when Mn==0) */
    seq_t result = M2n;
    if (Mn != 0) {
        seq_t sq = Mn * Mn;
        result -= sq;
    }
    return result;
}
