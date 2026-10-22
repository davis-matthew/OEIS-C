/*
 * A338979  a(n) = Sum_{k=0..n} n^k * binomial(n,k) * Catalan(k).
 * https://oeis.org/A338979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^k * binomial(n,k) * Catalan(k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;      /* convert index to seq_t once */
    seq_t sum     = (seq_t)0;    /* running total   */
    seq_t pow_n   = (seq_t)1;    /* n^0 initially   */
    seq_t binom   = (seq_t)1;    /* C(n,0)          */
    seq_t catalan = (seq_t)1;    /* Catalan(0)      */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* add current term */
        sum += catalan * binom * pow_n;

        /* prepare for next k, if any */
        if (k < n)
        {
            /* powers of n */
            pow_n *= n_seq;

            /* binomial coefficient C(n,k+1) from C(n,k) */
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);

            /* Catalan recurrence:
             * C_{k+1} = C_k * (4(k+1) - 2) / (k + 2)  */
            catalan = catalan * (seq_t)(4 * (k + 1) - 2) / (seq_t)(k + 2);
        }
    }

    return sum;
}
