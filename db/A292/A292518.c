/*
 * A292518  Expansion of Product_{k>=1} (1 - x^(k*(k+1)/2)).
 * https://oeis.org/A292518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Product_{k>=1}(1 - x^{k(k+1)/2})
 * Range:     i8 n <= 411, i16 n <= 8587, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A292518: the signed number of partitions
   of n into distinct triangular numbers.  Uses a simple DP over
   triangular numbers up to n.  Works for any seq_t and idx_t types
   supported by the build. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp)
        return 0;          /* allocation failure – return 0 */

    dp[0] = 1;              /* base case a(0) = 1 */

    /* Process each triangular number t = k*(k+1)/2 <= n once */
    for (idx_t k = 1;; ++k)
    {
        seq_t t = (seq_t)k * (k + 1) / 2;   /* triangular number */
        if (t > (seq_t)n)
            break;                        /* no more needed */

        idx_t tIdx = (idx_t)t;              /* for indexing */
        /* Update DP from high to low to avoid reuse of t in the same round */
        for (idx_t j = n; j >= tIdx; --j)
        {
            dp[j] = dp[j] - dp[j - tIdx];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
