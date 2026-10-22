/*
 * A306708  Expansion of Sum_{k>=0} x^(k^2) / Product_{j=1..k} (1 + x^j)^j.
 * https://oeis.org/A306708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Sum_{k>=0} x^(k^2) /
 *            Product_{j=1..k} (1 + x^j)^j
 * Range:     i8 n <= 40, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the series by dynamic convolution of reciprocal polynomials. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

seq_t a(idx_t n)
{
    idx_t N = n;
    /* Allocate coefficient arrays */
    seq_t *dp   = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *tmp  = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *res  = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));

    dp[0] = 1;          /* product up to k=0 is 1 */
    res[0] = 1;         /* k=0 contributes to a(0) only */

    idx_t Kmax = (idx_t)floor(sqrt((double)N));
    for (idx_t k = 1; k <= Kmax; ++k) {
        idx_t tmax = N / k;                       /* maximum exponent of t for this k */
        seq_t *coeff = (seq_t*)malloc((size_t)(tmax + 1) * sizeof(seq_t));
        seq_t binom = 1;
        coeff[0] = 1;                              /* binom(k-1,0) = 1 */

        /* Pre‑compute coefficients of 1/(1 + x^k)^k:
           coeff[t] = (-1)^t * binom(k + t - 1, t)   */
        for (idx_t t = 1; t <= tmax; ++t) {
            binom = binom * (k + t - 1) / t;        /* binomial recurrence */
            coeff[t] = (t & 1) ? -binom : binom;
        }

        /* Convolution: dp (previous product) * coeff (reciprocal of new factor) */
        memset(tmp, 0, (size_t)(N + 1) * sizeof(seq_t));
        for (idx_t s0 = 0; s0 <= N; ++s0) {
            seq_t v = dp[s0];
            if (v == 0) continue;
            idx_t rem = N - s0;
            idx_t maxT = rem / k;
            for (idx_t t = 0; t <= maxT; ++t) {
                idx_t idx = s0 + k * t;
                tmp[idx] += v * coeff[t];
            }
        }

        /* Swap dp and tmp buffers for next iteration */
        seq_t *old = dp;
        dp = tmp;
        tmp = old;

        /* Add contributions of this k to the result sequence */
        idx_t ksq = k * k;
        for (idx_t i = ksq; i <= N; ++i) {
            res[i] += dp[i - ksq];
        }

        free(coeff);
    }

    seq_t ans = res[N];
    free(dp);
    free(tmp);
    free(res);
    return ans;
}
