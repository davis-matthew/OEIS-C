/*
 * A388202  a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(n+2,k).
 * https://oeis.org/A388202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(n+2,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A388202 in O(n) time for arbitrary-precision seq_t. */
seq_t a(idx_t n)
{
    /* iterate over k = 0..n, updating the binomial coefficients and power of 3 */
    seq_t result = 0;           /* accumulator for the sum */
    seq_t binom_n_k = 1;        /* binomial(n, k), initialized at k = 0 */
    seq_t binom_np2_k = 1;      /* binomial(n+2, k), initialized at k = 0 */
    seq_t pow3 = 1;             /* 3^k, initialized at k = 0 */

    /* loop over k */
    for (idx_t k = 0; k <= n; ++k) {
        /* add current term to the result */
        seq_t term = binom_n_k * binom_np2_k;   /* (n choose k) * (n+2 choose k) */
        term = term * pow3;                   /* multiply by 3^k */
        result = result + term;               /* accumulate */

        /* break if we reached the last k */
        if (k == n)
            break;

        /* update binomial coefficients for next k */
        idx_t denom = k + 1;                   /* denominator for binomial recurrence */
        seq_t mult_n_k = (seq_t)(n - k);       /* numerator for binomial(n, k+1) */
        binom_n_k = (binom_n_k * mult_n_k) / (seq_t)denom;

        seq_t mult_np2_k = (seq_t)(n + 2 - k); /* numerator for binomial(n+2, k+1) */
        binom_np2_k = (binom_np2_k * mult_np2_k) / (seq_t)denom;

        /* update power of 3 */
        pow3 = pow3 * (seq_t)3;
    }

    return result;
}
