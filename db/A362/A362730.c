/*
 * A362730  a(n) = [x^n] E(x)^n where E(x) = exp( Sum_{k >= 1}
 *          binomial(2*k,k)^2*x^k/k ).
 * https://oeis.org/A362730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of x^n in (exp (sum_{k>=1} binom(2k,k)^2
 *            x^k/k))^n. Recurrence (derived from the differential equation of
 *            the exponential generating function): a_{n,0} = 1 a_{n,m} =
 *            (n/m) * sum_{k=1}^{m} binom(2k,k)^2 * a_{n,m-k} where a_{n,m} is
 *            the coefficient of x^m in the series for f...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Precompute squared binomial coefficients c[k] = binom(2k,k)^2 for k=1..n. */
    seq_t *c = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!c)
        return 0;        /* allocation failed, return 0 as error code */
    c[0] = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t b = binomial(2 * k, k);
        c[k] = b * b;
    }

    /* DP array for coefficients a_{n,m} */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) {
        free(c);
        return 0;
    }
    dp[0] = 1;   /* a_{n,0} = 1 */

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += c[k] * dp[m - k];
        }
        /* Compute a_{n,m} = (n * sum) / m; division is exact by construction. */
        dp[m] = ((seq_t)n * sum) / (seq_t)m;
    }

    seq_t result = dp[n];

    free(dp);
    free(c);
    return result;
}
