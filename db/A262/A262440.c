/*
 * A262440  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+k-1,n-k).
 * https://oeis.org/A262440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k) * binomial(n+k-1, n-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if ((idx_t)0 == n) {
        return (seq_t)1;
    }

    /* Convert n to seq_t once for efficiency */
    const seq_t ns = (seq_t)n;

    /* k = 0 term: zero (except for n=0 handled above) */
    seq_t sum = (seq_t)0;

    /* k = 1 term */
    seq_t binom_nk = ns;          /* binomial(n,1) */
    seq_t B_k     = ns;           /* binomial(n+1-1, n-1) = binomial(n, n-1) = n */
    sum += binom_nk * B_k;

    if ((idx_t)1 == n) {
        return sum;               /* early exit for n = 1 */
    }

    /* For the recurrence we keep the previous B value */
    seq_t B_prev = B_k;           /* B_1 */

    /* Iterate for k = 2 .. n */
    for (idx_t k = 2; k <= n; ++k) {
        /* Update binomial(n,k) from binomial(n,k-1) */
        binom_nk = binom_nk * (ns - (seq_t)(k - 1)) / (seq_t)k;

        /* Compute B_k using the recurrence:
           B_k = B_{k-1} * (n + k - 1) * (n - k + 1) / ((2k-1)*(2k-2)) */
        const seq_t mult1 = ns + (seq_t)(k - 1);
        const seq_t mult2 = ns - (seq_t)(k - 1);
        const seq_t num   = B_prev * mult1 * mult2;
        const seq_t denom = ((seq_t)(2 * k - 1)) * ((seq_t)(2 * k - 2));
        B_k = num / denom;

        sum += binom_nk * B_k;

        B_prev = B_k;  /* prepare for next iteration */
    }

    return sum;
}
