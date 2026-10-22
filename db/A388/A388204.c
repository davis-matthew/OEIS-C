/*
 * A388204  a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(n+4,k).
 * https://oeis.org/A388204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(n+4,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 0;           /* accumulated sum   */
    seq_t binom_n = 1;       /* binomial(n,k)     */
    seq_t binom_n4 = 1;      /* binomial(n+4,k)   */
    seq_t pow3 = 1;          /* 3^k               */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow3 * binom_n * binom_n4;
        res += term;
        if (k == n) break;
        pow3 *= 3;
        binom_n = binom_n * (seq_t)(n - k) / (seq_t)(k + 1);
        binom_n4 = binom_n4 * (seq_t)(n + 4 - k) / (seq_t)(k + 1);
    }
    return res;
}
