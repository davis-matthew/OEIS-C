/*
 * A303190  a(n) = [x^n] Product_{k=1..n} 1/(1 + (n - k + 1)*x^k).
 * https://oeis.org/A303190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] Π_{k=1..n} 1/(1 + (n - k + 1) x^k) The
 *            implementation computes the coefficient by an in-place DP: dp[i]
 *            contains the coefficient of x^i after processing factors for k =
 *            1..current. Updating a factor 1/(1 + c x^k) is equivalent to
 *            dp[i] -= c * dp[i - k] for all i ≥ k. The recurrence r...
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Allocate coefficient array [0..n] */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) abort();              /* Ensure we don't proceed on OOM */

    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                     /* Initial product is 1 */

    /* Process each factor 1/(1 + c x^k) with k = 1..n, c = n - k + 1 */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t c = (seq_t)(n - k + 1);
        for (idx_t i = k; i <= n; ++i) {
            dp[i] -= c * dp[i - k];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
