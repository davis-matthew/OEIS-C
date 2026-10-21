/*
 * A276235  Number of triangular partitions of n of order 6.
 * https://oeis.org/A276235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in * 1 /
 *            ((1-x)^6*(1-x^3)^5*(1-x^5)^4*(1-x^7)^3*(1-x^9)^2*(1-x^11)) *
 *            (triangular partitions of n of order 6, A276235)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 45, u64 n <= 216,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache   = NULL;
    static idx_t cache_max = 0;

    /* if we already have computed the required term, return it */
    if (cache && n <= cache_max)
        return cache[n];

    /* allocate new cache large enough for maximum n needed */
    if (cache != NULL)
        free(cache);
    cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!cache) return (seq_t)0;   /* out of memory */

    /* temporary DP arrays */
    seq_t *dp   = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp2  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp || !dp2) return (seq_t)0;

    /* initial condition: only 0 can be represented with no parts */
    dp[0] = 1;

    /* part sizes and corresponding multiplicities */
    const idx_t part_sizes[6] = {1, 3, 5, 7, 9, 11};
    const idx_t mults[6]     = {6, 5, 4, 3, 2, 1};

    for (idx_t p = 0; p < 6; ++p) {
        idx_t k   = part_sizes[p];
        idx_t m   = mults[p];
        idx_t max_t = n / k;

        /* pre‑compute binomial coefficients binom(t+m-1, m-1) */
        seq_t *coeff = (seq_t *)calloc((size_t)(max_t + 1), sizeof(seq_t));
        if (!coeff) return (seq_t)0;
        coeff[0] = 1;
        for (idx_t t = 1; t <= max_t; ++t) {
            /* coeff[t] = coeff[t-1] * (t + m - 1) / t   (integer division) */
            coeff[t] = (coeff[t - 1] * (seq_t)(t + m - 1)) / (seq_t)t;
        }

        /* reset dp2 to zero */
        for (size_t i = 0; i <= n; ++i) dp2[i] = 0;

        /* convolution with part (1 - x^k)^(-m) */
        for (idx_t i = 0; i <= n; ++i) {
            seq_t cur = dp[i];
            if (cur == 0) continue;        /* skip zero terms */
            idx_t s   = i;
            idx_t t   = 0;
            for (; s <= n; s += k, ++t)
                dp2[s] += cur * coeff[t];
        }

        /* swap dp and dp2 for next iteration */
        seq_t *tmp = dp;
        dp = dp2;
        dp2 = tmp;

        free(coeff);
    }

    /* copy result to cache for future queries */
    for (size_t i = 0; i <= n; ++i)
        cache[i] = dp[i];

    /* free temporary arrays */
    free(dp);
    free(dp2);

    cache_max = n;
    return cache[n];
}
