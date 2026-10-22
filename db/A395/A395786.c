/*
 * A395786  Expansion of (Sum_{k>=0} binomial(4*k+2,k) * x^k)^3.
 * https://oeis.org/A395786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (k+1)*3^k*binomial(4*n+8, n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* N = 4*n + 8  (index for the binomial arguments) */
    idx_t N = 4 * n + 8;

    /* Compute binomial(N, n) iteratively to avoid huge intermediate
     * factorials.  binom will hold binomial(N, n-k) during the loop. */
    seq_t binom = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        binom *= (seq_t)(N - (k - 1));
        binom /= (seq_t)k;
    }

    seq_t power = (seq_t)1; /* 3^0 */
    seq_t sum   = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = (k+1) * 3^k * binom(N, n-k) */
        seq_t term = (seq_t)(k + 1) * power * binom;
        sum += term;

        if (k < n)
        {
            /* advance 3^k to 3^(k+1) */
            power *= (seq_t)3;

            /* update binom to binomial(N, n-(k+1))
             * binom_next = binom * (n-k) / (N - n + k + 1)  */
            idx_t num = n - k;
            idx_t den = N - n + k + 1; /* = 3*n + 9 + k */
            binom = binom * (seq_t)num;
            binom = binom / (seq_t)den;
        }
    }

    return sum;
}
