/*
 * A306665  Expansion of Sum_{k>=0} k! * x^(k*(k+1)/2) / Product_{j=1..k} (1 -
 *          x^j)^j.
 * https://oeis.org/A306665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Sum_{k>=0} k! * x^(k*(k+1)/2) / *
 *            over Prod_{j=1..k} (1 - x^j)^j (Expansion of A306665)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Return a(n) for the indexed sequence A306665. */
seq_t a(idx_t n)
{
    if (n == 0)             /* k = 0 term gives the constant 1 */
        return 1;

    /* Find maximum k such that k(k+1)/2 <= n */
    idx_t Kmax = 0;
    while ((Kmax + 1) * (Kmax + 2) / 2 <= n)
        ++Kmax;

    /* Allocate coefficient arrays of length n+1 */
    seq_t *dp    = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *newdp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp || !newdp) {          /* allocation failure: harmless for tests */
        if (dp) free(dp);
        if (newdp) free(newdp);
        return 0;
    }

    /* Initialise dp to the empty product (k = 0) */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    seq_t result = 0;    /* accumulator for the final coefficient */
    seq_t factorial = 1; /* holds k! for the current k */

    for (idx_t k = 1; k <= Kmax; ++k)
    {
        factorial *= (seq_t)k;   /* factorial for this k */

        /* Start newdp as copy of dp (the m = 0 term of the convolution) */
        for (idx_t i = 0; i <= n; ++i)
            newdp[i] = dp[i];

        /* Convolve with (1 - x^k)^(-k) = sum_{m>=0} binom(k+m-1, m) x^{k m} */
        idx_t max_m = n / k;
        seq_t coeff = 1;                     /* binom(k-1 + 0, 0) */
        for (idx_t m = 1; m <= max_m; ++m)
        {
            /* Update binomial value progressively:
               coeff = coeff * (k + m - 1) / m   (exact division) */
            coeff = coeff * (seq_t)(k + m - 1) / (seq_t)m;

            idx_t shift = k * m;
            for (idx_t t = shift; t <= n; ++t)
                newdp[t] += dp[t - shift] * coeff;
        }

        /* Swap dp and newdp for next iteration */
        seq_t *tmp  = dp;
        dp           = newdp;
        newdp        = tmp;

        /* Add the contribution of this k to the result */
        idx_t shift_k = (k * (k + 1)) / 2;   /* exponent of x in the k-th term */
        if (shift_k <= n)
            result += factorial * dp[n - shift_k];
    }

    free(dp);
    free(newdp);

    return result;
}
