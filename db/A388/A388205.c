/*
 * A388205  a(n) = Sum_{k=0..n} 4^k * binomial(n,k) * binomial(n+2,k).
 * https://oeis.org/A388205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial(n,k) * binomial(n+2,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom_nk = 1;      /* binomial(n,0)   */
    seq_t binom_n2k = 1;     /* binomial(n+2,0) */
    seq_t pow4 = 1;          /* 4^0             */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = 4^k * binomial(n,k) * binomial(n+2,k) */
        seq_t term = pow4 * binom_nk * binom_n2k;
        sum += term;

        /* Prepare next values for k+1, if any */
        if (k < n) {
            pow4 *= ((seq_t)4);
            binom_nk = binom_nk * (seq_t)(n - k) / (seq_t)(k + 1);
            binom_n2k = binom_n2k * (seq_t)(n + 2 - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}
