/*
 * A292132  Main diagonal of A292131.
 * https://oeis.org/A292132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..n} (1 - n*x^k)
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 26, i64 n <= 61, i128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained by expanding the product.  For each term
 * (1 - n*x^k), we either contribute 1 (by not using the term) or
 * (−n)x^k (by selecting it).  Thus the coefficient of x^n is the
 * sum of (−n)^|S| over all subsets S of {1,…,n} whose indices sum to n. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return 1;

    /* Allocate an array of n+1 seq_t's and zero all entries. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return 0;                     /* allocation failure – unlikely */
    memset(dp, 0, (n + 1) * sizeof(seq_t));
    dp[0] = 1;                         /* empty subset contributes 1 */

    /* Each factor is (1 - n*x^k).  The coefficient for the
     * “use this term” branch is -n.  Since the same n is used in
     * every factor, we can apply the classic knapsack‑style DP
     * for each k. */
    seq_t coeff = -(seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t s = n; s >= k; --s) {
            dp[s] += dp[s - k] * coeff;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
