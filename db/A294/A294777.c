/*
 * A294777  Expansion of Product_{k>=1} (1 + x^(2*k-1))^(k*(k-1)/2).
 * https://oeis.org/A294777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 36, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = coefficient of x^n in
   ∏_{k≥1} (1 + x^(2*k-1))^(k*(k-1)/2)
   (OEIS A294777, offset 0).  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case directly */
    if (n == 0) return (seq_t)1;

    /* Length of the coefficient array */
    size_t sz = (size_t)n + 1;

    /* Allocate two working arrays of seq_t */
    seq_t *dp   = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp || !next) {
        free(dp); free(next);
        return (seq_t)0;   /* allocation failure */
    }

    /* Initialise dp[0] = 1, others 0 */
    for (size_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;

    /* Process each odd exponent w = 2*k-1 */
    for (idx_t w = 1; w <= n; w += 2) {
        idx_t k = (w + 1) / 2;
        seq_t m = (seq_t)k * (seq_t)(k - 1) / 2;   /* multiplicity */

        /* t must satisfy t*w ≤ n and t ≤ m */
        idx_t tmax = n / w;
        if (tmax > (idx_t)m) tmax = (idx_t)m;
        if (tmax == 0) continue;                 /* this weight has no effect */

        /* Pre‑compute binomial coefficients C(m,t) for t=0..tmax */
        seq_t *binom = (seq_t *)malloc((size_t)(tmax + 1) * sizeof(seq_t));
        if (!binom) {
            free(dp); free(next);
            return (seq_t)0;   /* allocation failure */
        }
        binom[0] = (seq_t)1;
        for (idx_t t = 1; t <= tmax; ++t) {
            seq_t prev = binom[t - 1];
            seq_t factor = m - (seq_t)(t - 1);
            seq_t num    = prev * factor;
            binom[t] = num / (seq_t)t;          /* exact division */
        }

        /* Start with a copy of the current polynomial */
        for (size_t i = 0; i < sz; ++i) next[i] = dp[i];

        /* Add contributions for all t ≥ 1 */
        for (idx_t t = 1; t <= tmax; ++t) {
            seq_t coeff = binom[t];
            idx_t start = t * w;
            for (idx_t j = start; j <= n; ++j) {
                next[j] += dp[j - start] * coeff;
            }
        }

        free(binom);

        /* Prepare for next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(next);
    return result;
}
