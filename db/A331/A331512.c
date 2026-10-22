/*
 * A331512  a(n) = Sum_{k=0..n} n^(n-k) * (n+k+1) * binomial(n,k) *
 *          binomial(n+k,k).
 * https://oeis.org/A331512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (n+1)^k * (k+1) * binomial(n+1,k+1)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;
    seq_t binom  = (seq_t)(n + 1); /* binomial(n+1,1) */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = power * (seq_t)(k + 1);
        term = term * binom * binom;
        result += term;

        /* advance to the next k */
        power *= (seq_t)(n + 1);

        if (k < n)
        {
            /* binom = binom * (n - k) / (k + 2) */
            seq_t num = (seq_t)(n - k);
            seq_t den = (seq_t)(k + 2);
            binom = (binom * num) / den;
        }
    }

    return result;
}
