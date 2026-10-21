/*
 * A246462  a(n) = Sum_{k=0..n} (2k+1)*C(n,k)^2*C(n+k,k)^2, where C(n,k)
 *          denotes the binomial coefficient n!/(k!*(n-k)!).
 * https://oeis.org/A246462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2k+1)*C(n,k)^2*C(n+k,k)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for unsigned integer index n (idx_t) using arbitrary precision
   arithmetic. This implementation evaluates the sum directly in O(n) steps,
   updating the binomial coefficients iteratively to avoid factorials or
   high‑order intermediate values.  */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t binom_n_k = 1;          /* C(n,0) */
    seq_t binom_nk_k = 1;         /* C(n+0,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = (2k+1) * binom_n_k^2 * binom_nk_k^2 */
        seq_t a2 = binom_n_k * binom_n_k;
        seq_t b2 = binom_nk_k * binom_nk_k;
        seq_t term = a2 * b2;
        term *= (seq_t)(2 * k + 1);
        result += term;

        /* Prepare the next binomials for k+1 (if any) */
        if (k < n)
        {
            /* C(n,k+1) = C(n,k) * (n-k) / (k+1) */
            seq_t num = binom_n_k * (seq_t)(n - k);
            binom_n_k = num / (seq_t)(k + 1);

            /* C(n+k+1,k+1) = C(n+k,k) * (n+k+1) / (k+1) */
            num = binom_nk_k * (seq_t)(n + k + 1);
            binom_nk_k = num / (seq_t)(k + 1);
        }
    }

    return result;
}
