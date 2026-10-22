/*
 * A369012  Expansion of (1/x) * Series_Reversion( x * (1-x/(1-x))^3 ).
 * https://oeis.org/A369012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * sum_{k=0..n} binomial(3*n+k+2, k) *
 *            binomial(n-1, n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = (1/(n+1)) * sum_{k=1..n} binomial(3*n+k+2, k) * binomial(n-1, k-1) */

#ifdef __cplusplus
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t sum     = 0;
    seq_t term1   = 1; /* binomial(3*n+3,1) for k=1 */
    seq_t term2   = 1; /* binomial(n-1,0)   for k=1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update term1 = binomial(3*n + k + 2, k) iteratively */
        seq_t mult1 = (seq_t)3 * n + k + 2;
        term1 = term1 * mult1 / (seq_t)k;

        /* Update term2 = binomial(n-1, k-1) iteratively (skip for k==1) */
        if (k > 1)
        {
            seq_t mult2 = (seq_t)n - (seq_t)k + 1; /* (n - (k-1)) */
            term2 = term2 * mult2 / (seq_t)(k - 1);
        }

        sum += term1 * term2;
    }
    return sum / ((seq_t)n + 1);
}
#else
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Use 128‑bit arithmetic for intermediate values */
    unsigned __int128 sum   = 0;
    unsigned __int128 term1 = 1; /* binomial(3*n+3,1) for k=1 */
    unsigned __int128 term2 = 1; /* binomial(n-1,0)   for k=1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        unsigned __int128 mult1 = (unsigned __int128)3 * n + k + 2;
        term1 = term1 * mult1 / k;

        if (k > 1)
        {
            unsigned __int128 mult2 = (unsigned __int128)n - k + 1;
            term2 = term2 * mult2 / (k - 1);
        }

        sum += term1 * term2;
    }

    unsigned __int128 result = sum / (n + 1);
    return (seq_t)result;  /* cast is safe when the result fits in seq_t */
}
#endif
