/*
 * A303188  a(n) = [x^n] Product_{k=1..n} (1 + (n - k + 1)*x^k).
 * https://oeis.org/A303188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in the product ∏_{k=1..n} (1 +
 *            (n-k+1)·x^k)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 64, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A303188.
 * Uses a simple dynamic‑programming approach that runs in O(n²) time and
 * O(n) memory.  It works for any seq_t and idx_t supplied by the
 * surrounding build system. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 0, with a(0) = 1. */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Allocate an array to hold the intermediate polynomial coefficients.
     * dp[j] will contain the coefficient of x^j after the first k
     * factors have been processed.  We only need indices up to n. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;            /* allocation failed – return 0 as sentinel */

    /* Initialise: coefficient of x^0 is 1, all other coefficients 0. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Process each factor (1 + (n-k+1)·x^k) for k = 1..n. */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t weight = (seq_t)(n - k + 1);

        /* Update coefficients backwards to avoid using updated values
         * from the current factor too early. */
        for (idx_t j = n; j >= k; --j) {
            dp[j] = dp[j] + dp[j - k] * weight;
            if (j == k)          /* avoid unsigned underflow when j-- becomes 0 */
                break;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
