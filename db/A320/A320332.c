/*
 * A320332  Expansion of Product_{0 < i < j < k} (1 + x^(i^2 + j^2 + k^2)).
 * https://oeis.org/A320332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of the set { (i,j,k) | 0<i<j<k } whose
 *            * summed weights i² + j² + k² equal n. Equivalently, the
 *            coefficient of * x^n in the product ∏_{0<i<j<k} (1 + x^(i² + j²
 *            + k²))
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for unsigned n. */
static idx_t isqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0 || mid <= n / mid)   /* mid*mid <= n, avoid overflow */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                  /* allocation failure – unlikely */

    /* Initialise DP: dp[0] = 1, others 0 */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Precompute squares up to sqrt(n) */
    idx_t lim = isqrt(n);
    idx_t *sq = (idx_t *)malloc((lim + 1) * sizeof(idx_t));
    if (!sq) {
        free(dp);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= lim; ++i)
        sq[i] = i * i;

    /* Enumerate all triples i < j < k */
    for (idx_t i = 1; i <= lim; ++i) {
        for (idx_t j = i + 1; j <= lim; ++j) {
            for (idx_t k = j + 1; k <= lim; ++k) {
                seq_t t = (seq_t)sq[i] + (seq_t)sq[j] + (seq_t)sq[k];
                if (t > (seq_t)n)
                    continue;
                idx_t tidx = (idx_t)t;
                /* descending DP update: dp[s] += dp[s-t] */
                for (idx_t s = n; s >= t; --s)
                    dp[s] += dp[s - tidx];
            }
        }
    }

    seq_t result = dp[n];

    free(sq);
    free(dp);
    return result;
}
