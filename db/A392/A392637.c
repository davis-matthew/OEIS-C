/*
 * A392637  Number of planar partitions of n with strictly decreasing columns
 *          and where parts decrease by at most 1 along each row.
 * https://oeis.org/A392637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - x^(6k-4)) / (1 -
 *            x^k)^ceil(k/6)
 * Range:     u8 n <= 18, u16 n <= 41, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t N = n;
    /* Allocate working arrays for the series expansion of the denominator.
       dp holds the current series, newdp is used for each multiplication. */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *newdp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp || !newdp) {
        free(dp);
        free(newdp);
        return (seq_t)0;
    }

    /* Initialize dp to the series of 1. */
    for (idx_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Build the denominator series ∏ (1 - x^k)^(-ceil(k/6)) term by term. */
    for (idx_t k = 1; k <= N; ++k) {
        int a_k = (int)((k + 5) / 6);          /* ceil(k/6)  */
        idx_t max_j = N / k;                   /* highest j with j*k ≤ N */

        /* Zero the newdp array. */
        for (idx_t i = 0; i <= N; ++i) newdp[i] = 0;

        /* w[j] = binom(a_k + j - 1, j). Compute multiplicatively. */
        seq_t w = 1;   /* w[0] */
        for (idx_t j = 0; j <= max_j; ++j) {
            idx_t shift = j * k;
            idx_t lim = N - shift;
            for (idx_t pos = 0; pos <= lim; ++pos) {
                newdp[pos + shift] += dp[pos] * w;
            }
            if (j < max_j) {
                /* Next binomial coefficient */
                w = w * (seq_t)(a_k + j) / (seq_t)(j + 1);
            }
        }

        /* Swap dp and newdp for the next iteration. */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    /* Build numerator coefficients c[j] for ∏ (1 - x^(6k-4)).
       c[j] can be negative; we store it in a signed 16 bit array.   */
    int16_t *c = (int16_t *)malloc((N + 1) * sizeof(int16_t));
    if (!c) {
        free(dp);
        free(newdp);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= N; ++i) c[i] = 0;
    c[0] = 1;

    for (idx_t k = 1; ; ++k) {
        idx_t m = 6 * k - 4;
        if (m > N) break;
        /* In-place update from high to low to avoid double counting.   */
        for (idx_t j = N; j >= m; --j) {
            c[j] = c[j] - c[j - m];
        }
    }

    /* Convolve the denominator series dp with the numerator coefficients c
       to obtain the final coefficient a(N).  Separate positive and negative
       contributions to keep the final subtraction safe. */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    for (idx_t i = 0; i <= N; ++i) {
        int16_t coeff = c[N - i];
        if (coeff > 0) {
            sum_pos += dp[i] * (seq_t)coeff;
        } else if (coeff < 0) {
            sum_neg += dp[i] * (seq_t)(-coeff);
        }
    }
    seq_t result = sum_pos - sum_neg;

    /* Clean up. */
    free(dp);
    free(newdp);
    free(c);

    return result;
}
