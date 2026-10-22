/*
 * A374615  a(n) = Sum_{k=0..n} (k/n)^3 * binomial(n,k)^5.
 * https://oeis.org/A374615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n-1,k)^3 * binomial(n,k)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    seq_t binom_nm1 = (seq_t)1; /* binomial(n-1,0) */
    seq_t binom_n   = (seq_t)1; /* binomial(n,0)   */

    for (idx_t k = (idx_t)0; k < n; ++k)
    {
        /* term = binom_nm1^3 * binom_n^2 */
        seq_t term = binom_nm1 * binom_nm1;      /* ^2 */
        term = term * binom_nm1;                /* ^3 */
        seq_t tmp  = binom_n * binom_n;         /* ^2 */
        term = term * tmp;

        sum = sum + term;

        if (k + (idx_t)1 < n)
        {
            /* update binom_nm1 to binomial(n-1, k+1) */
            binom_nm1 = binom_nm1 * (seq_t)(n - (idx_t)1 - k) / (seq_t)(k + (idx_t)1);
            /* update binom_n to binomial(n, k+1) */
            binom_n   = binom_n   * (seq_t)(n - k)            / (seq_t)(k + (idx_t)1);
        }
    }

    return sum;
}
