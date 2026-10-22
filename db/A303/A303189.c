/*
 * A303189  a(n) = [x^n] Product_{k=1..n} (1 - (n - k + 1)*x^k).
 * https://oeis.org/A303189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Π_{k=1..n} (1 - (n - k + 1)*x^k)
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 27, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case n = 0: an empty product gives 1. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate DP array of size n+1.  dp[s] will hold the
     * coefficient of x^s after processing the first k terms.
     */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;  /* allocation failure – treat as 0 */

    /* Initialization: start with polynomial "1". */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Outer loop over k = 1 .. n.  For each k we multiply the
     * current polynomial by (1 - c*x^k) with c = n-k+1.
     */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t coeff = (seq_t)(n - k + 1);   /* c = n-k+1, cast to seq_t */
        /* Update coefficients from high degree downwards to avoid
         * using newly updated values in the same iteration.
         */
        for (idx_t j = n; j >= k; --j)
        {
            dp[j] += (-coeff) * dp[j - k];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
