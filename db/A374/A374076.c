/*
 * A374076  Expansion of Product_{k>=1} 1 / (1 + x^(5*k-1)).
 * https://oeis.org/A374076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1 / (1 + x^(5*k‑1))
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is computed by a single‑pass DP: for each part v = 5*k‑1,
   a[i] -= a[i - v] for all i ≥ v.  The array is updated in place,
   so only O(n) memory is used. */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* n = 0 is trivial */
    if (n == 0)
        return (seq_t)1;

    /* Allocate array of size n+1 */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    assert(dp != NULL);

    /* Initialise: dp[0] = 1, others 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Process each allowed part v = 5*k‑1 (start at 4) */
    for (idx_t v = 4; v <= n; v += 5) {
        /* For i ≥ v: dp[i] -= dp[i‑v] using the updated dp values */
        for (idx_t i = v; i <= n; ++i)
            dp[i] = dp[i] - dp[i - v];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
