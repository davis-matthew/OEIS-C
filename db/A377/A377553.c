/*
 * A377553  Expansion of e.g.f. (1/x) * Series_Reversion( x/(1 + x*exp(x))^2
 *          ).
 * https://oeis.org/A377553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!/(n+1)) * sum_{k=0..n} k^(n-k) * binomial(2*n+2,
 *            k)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a power with binary exponentiation.  */
static seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0U)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Special case n=0 handled automatically below */
    idx_t N     = n;
    idx_t Nplus1 = n + 1U;
    idx_t N2    = 2U * n + 2U;   /* 2*n+2 */

    seq_t binom = (seq_t)1;      /* binomial(2*n+2, 0) */
    seq_t prod  = (seq_t)1;      /* n!/(n-k)! for current k (starts at k=0) */
    seq_t sum   = (seq_t)0;

    for (idx_t k = 0U; k <= N; ++k)
    {
        /* Compute k^(n-k) */
        seq_t pow_val = seq_pow_uint((seq_t)k, N - k);

        /* term = binom * k^(n-k) * prod / (n+1) */
        seq_t term = binom * pow_val;
        term /= Nplus1;
        term *= prod;

        sum += term;

        if (k == N)
            break;   /* no further updates after the last term */

        /* Update binomial for next k: binomial(N2, k+1) from binomial(N2, k) */
        binom = (binom * (N2 - k)) / (k + 1U);

        /* Update product n!/(n-(k+1))! from current product */
        prod *= (N - k);
    }

    return sum;
}
