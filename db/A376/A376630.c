/*
 * A376630  G.f.: Sum_{k>=0} x^(k*(k+1)/2) * Product_{j=1..k} (1 + x^(2*j-1)).
 * https://oeis.org/A376630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 434, u32 n <= 1379, u64 n <= 4761,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376630:
 *  a(n) = coefficient of x^n in
 *  Σ_{k≥0} x^{k(k+1)/2} Π_{j=1..k} (1 + x^{2j-1})
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* dp[e] = number of subsets of odd numbers {1,3,…,2k-1}
     * that sum to e, after processing the current k.
     * We grow the set of odd numbers one by one and, for each k,
     * add the contribution to the answer for the target n.
     */

    /* Allocate array of size n+1, initialized to zero. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* allocation failure – return zero as fallback */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;   /* empty subset for k = 0 */

    seq_t ans = (seq_t)0;

    for (idx_t k = 0; ; ++k) {
        /* Triangular offset for this k */
        idx_t offset = k * (k + 1) / 2;
        if (offset > n)
            break;

        idx_t e = n - offset;       /* remaining sum that must be made from odds */
        if (e <= n)
            ans += dp[e];

        /* Prepare dp for the next k by adding the next odd weight
           2k+1 (which is 1 for k=0, 3 for k=1, etc.). */
        idx_t w = 2 * k + 1;
        if (w <= n) {
            for (idx_t idx = n; idx >= w; --idx) {
                if (dp[idx - w] != (seq_t)0)
                    dp[idx] += dp[idx - w];
            }
        }
    }

    free(dp);
    return ans;
}
