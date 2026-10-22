/*
 * A309172  Expansion of Product_{k>=1} 1/(1 - (1 + x + x^2) * x^k).
 * https://oeis.org/A309172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k>=1} 1/(1-(1+x+x^2)x^k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic array of coefficients */
    static seq_t *dp   = NULL;
    static size_t  cap = 0;
    size_t needed = (size_t)n + 1;

    /* Allocate (or reallocate) the array.  The cast is required for C++ */
    if (cap < needed) {
        free(dp);
        dp = (seq_t*)realloc(NULL, needed * sizeof(seq_t));
        cap = needed;
    }

    /* initialise dp[0] = 1, others to 0 */
    for (size_t i = 0; i < needed; ++i) dp[i] = 0;
    dp[0] = 1;

    /* DP over part sizes k = 1..n.
       Each part k contributes an offset of k, k+1, or k+2.
       The forward loop allows unlimited multiplicity of each part. */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t s = k; s <= n; ++s) {
            seq_t add = dp[s - k];
            if (s >= k + 1) add += dp[s - k - 1];
            if (s >= k + 2) add += dp[s - k - 2];
            dp[s] += add;
        }
    }

    return dp[(size_t)n];
}
