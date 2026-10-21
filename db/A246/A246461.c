/*
 * A246461  a(n) = Sum_{k=0..n} ((2k+1)*C(n,k)*C(n+k,k))^2, where C(n,k)
 *          denotes the binomial coefficient n!/(k!*(n-k)!).
 * https://oeis.org/A246461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} ((2k+1)*C(n,k)*C(n+k,k))^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t c1 = 1;     /* C(n,0) */
    seq_t c2 = 1;     /* C(n+0,0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t factor = (seq_t)(2 * k + 1);
        seq_t term = factor * c1;
        term *= c2;
        term = term * term;
        sum += term;

        if (k < n) {
            /* update C(n,k+1) = C(n,k)*(n - k)/(k + 1) */
            c1 = c1 * (seq_t)(n - k);
            c1 = c1 / (seq_t)(k + 1);

            /* update C(n+k+1, k+1) = C(n+k, k)*(n + k + 1)/(k + 1) */
            c2 = c2 * (seq_t)(n + k + 1);
            c2 = c2 / (seq_t)(k + 1);
        }
    }
    return sum;
}
