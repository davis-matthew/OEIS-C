/*
 * A388135  a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n,k) * binomial(4*n+1,k).
 * https://oeis.org/A388135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n,k) * binomial(4*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3^n once */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 = pow3 * (seq_t)3;

    /* binomial(n,k) and binomial(4*n+1,k) start at k=0 */
    seq_t bin_n = 1;        /* C(n,0)  */
    seq_t bin_m = 1;        /* C(4n+1,0) */
    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = 3^(n-k) * C(n,k) * C(4n+1,k) */
        seq_t term = pow3 * bin_n * bin_m;
        result += term;

        if (k == n)          /* last term processed */
            break;

        /* Prepare values for next k */
        pow3 = pow3 / (seq_t)3;

        bin_n = bin_n * (seq_t)(n - k) / (seq_t)(k + 1);
        bin_m = bin_m * (seq_t)(4 * n + 1 - k) / (seq_t)(k + 1);
    }

    return result;
}
