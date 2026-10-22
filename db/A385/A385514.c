/*
 * A385514  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(2*n+1,k) *
 *          binomial(2*n-k,n-k).
 * https://oeis.org/A385514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(n+k, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;   /* binomial(n + k, k) */
    seq_t pow3  = 1;   /* 3^k */

    for (idx_t k = 0; k <= n; ++k) {
        if (k != 0) {
            /* binom(n+k, k) = binom(n+k-1, k-1) * (n+k) / k */
            binom = binom * (n + k) / k;
            pow3 = pow3 * 3;
        }
        /* term = 3^k * binomial(n+k, k) */
        seq_t term = binom * pow3;
        sum += term;
    }

    return sum;
}
