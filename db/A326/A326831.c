/*
 * A326831  Expansion of Product_{i>=2, j>=2} (1 + x^(i*j))^j.
 * https://oeis.org/A326831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in * ∏_{m>=4} (1 + x^m)^{σ(m) – 1 –
 *            m}, * where σ(m) is the sum of all positive divisors of m. *
 *            This implements the definition directly using dynamic *
 *            programming over the polynomial variables. The routine * works
 *            for any build‑supplied integral or arbitrary‑precision...
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 87, u64 n <= 209,
 *            u128 n <= 527, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
# include <new>
#endif

seq_t a(idx_t n)
{
    /* Simple case: the result is stored in a dynamically
     * allocated array of length n+1.  For C++ we use new/delete,
     * for C we use calloc/free. */
    idx_t sz = n + 1;

#ifdef __cplusplus
    seq_t *dp     = new seq_t[sz];
    seq_t *newdp  = new seq_t[sz];
#else
    seq_t *dp     = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *newdp  = (seq_t *)calloc(sz, sizeof(seq_t));
#endif
    if (!dp || !newdp)  /* allocation failure → 0 */
    {
#ifdef __cplusplus
        delete[] dp;
        delete[] newdp;
#else
        free(dp);
        free(newdp);
#endif
        return (seq_t)0;
    }

    /* initialise dp to the constant term 1, the rest are 0. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i < sz; ++i) dp[i] = (seq_t)0;

    /* Compute divisor sums σ(m) for m = 1..n. */
    idx_t *sigma = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!sigma)
    {
#ifdef __cplusplus
        delete[] dp;
        delete[] newdp;
#else
        free(dp);
        free(newdp);
#endif
        return (seq_t)0;
    }

    for (idx_t i = 1; i <= n; ++i)
    {
        for (idx_t j = i; j <= n; j += i) sigma[j] += i;
    }

    /* Main DP over m = 4..n.
     * For each m we multiply the current polynomial dp
     * by (1 + x^m)^{E(m)} where E(m) = σ(m) – 1 – m.      */
    for (idx_t m = 4; m <= n; ++m)
    {
        idx_t Em_idx = sigma[m] - 1 - m;       /* Exponent for this m */
        if (Em_idx == 0) continue;            /* Factor is 1, skip */

        seq_t Em_seq = Em_idx;                 /* Convert to seq_t */
        idx_t kmax = n / m;                    /* Max k that keeps offset <= n */
        if (kmax > Em_idx) kmax = Em_idx;      /* Can't use more than Em copies */

        /* Copy old polynomial into newdp (k = 0 part). */
        for (idx_t i = 0; i < sz; ++i) newdp[i] = dp[i];

        /* Accumulate contributions for k = 1..kmax. */
        seq_t binom = (seq_t)1;                /* Current binomial coefficient */
        for (idx_t k = 1; k <= kmax; ++k)
        {
            /* Update binomial coefficient:
             * binom = binom * (Em - (k-1)) / k  */
            binom = binom * (Em_seq - (seq_t)(k - 1)) / (seq_t)k;

            idx_t offset = m * k;             /* Exponent shift */
            for (idx_t y = offset; y <= n; ++y)
            {
                newdp[y] += dp[y - offset] * binom;
            }
        }

        /* Prepare for next iteration: swap dp and newdp. */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];

    /* Free memory */
    free(sigma);
#ifdef __cplusplus
    delete[] dp;
    delete[] newdp;
#else
    free(dp);
    free(newdp);
#endif

    return result;
}
