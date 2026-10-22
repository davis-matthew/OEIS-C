/*
 * A337057  a(n) = exp(-n) * Sum_{k>=0} (k - n)^n * n^k / k!.
 * https://oeis.org/A337057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0 For n≥2: a(n) = n * Σ_{s=2}^{n} C(n-1, s-1) *
 *            a(n-s) (1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of the recurrence (1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* index type fits into size_t for the array size */
    size_t N = (size_t)n;

    /* Allocate array for DP values */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;          /* out of memory – unlikely in tests */

    /* Base cases */
    dp[0] = (seq_t)1;
    if (N >= 1)
        dp[1] = (seq_t)0;

    /* Precompute a(k) for k = 2 .. N */
    for (size_t k = 2; k <= N; ++k)
    {
        seq_t sum = (seq_t)0;

        /* Current binomial coefficient C(k-1, 1) */
        seq_t binom = (seq_t)(k - 1);

        /* Loop over s = 2 .. k
           Here r = s-1 runs from 1 .. k-1   */
        for (size_t r = 1; r < k; ++r)
        {
            /* term = binom * n * dp[k - s] with s = r+1 */
            seq_t term = binom * (seq_t)n * dp[k - r - 1];
            sum += term;

            /* Prepare next binomial coefficient:
               C(k-1, r+1) = C(k-1, r) * (k-1-r) / (r+1) */
            if (r < k - 1)
            {
                seq_t num = binom * (seq_t)(k - 1 - r);
                binom = num / (seq_t)(r + 1);
            }
        }

        dp[k] = sum;
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
