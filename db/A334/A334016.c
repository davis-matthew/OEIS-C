/*
 * A334016  Table read by antidiagonals upward: T(n,k) is the number of ways
 *          to move a chess queen from (1,1) to (n,k) in the first quadrant
 *          using only right, diagonal up-right, and diagonal up-left moves.
 * https://oeis.org/A334016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(1,1)=1
 *            T(n,k)=∑_{i=1}^{k-1}T(n+i,k-i)+∑_{i=1}^{min(n,k)-1}T(n-i,k-i)+∑_{i=1}^{n-1}T(n-i,k)
 *            a(n) is T(n,k) read by antidiagonals upward (n,k where n+k is
 *            constant)
 * Range:     u8 n <= 18, u16 n <= 35, u32 n <= 102, u64 n <= 297,
 *            u128 n <= 988, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t cur_max_sum = 0;      /* largest diagonal sum already computed   */
    static seq_t *dp = NULL;           /* memoized table                         */
    static idx_t stride = 0;           /* table stride (≥ cur_max_sum)           */

    /* find diagonal sum s such that the n-th element lies on it   */
    idx_t s = 2;       /* minimal sum (1,1)                        */
    idx_t cum = 0;     /* entries before current diagonal          */
    while (cum + (s - 1) < n) {          /* accumulate until we cover n   */
        cum += s - 1;
        s++;
    }
    /* cum now equals the number of elements before diagonal s    */
    idx_t pos = n - cum;      /* 1‑based position inside diagonal       */
    idx_t n_coord = s - pos;
    idx_t k_coord = pos;

    /* rebuild table if a larger diagonal is required             */
    if (s > cur_max_sum) {
        if (dp != NULL) free(dp);

        stride = s + 1;              /* stride must accommodate indices 0..s   */
        size_t size = (size_t)stride * (size_t)stride;
        dp = (seq_t *)malloc(size * sizeof(seq_t));
        for (size_t i = 0; i < size; ++i) dp[i] = 0;

        /* base case */
        dp[1 * stride + 1] = 1;

        /* compute table entries for all sums from 3 to s          */
        for (idx_t sum = 3; sum <= s; ++sum) {
            for (idx_t n_curr = sum - 1; n_curr >= 1; --n_curr) {
                idx_t k_curr = sum - n_curr;   /* k ≥ 1 automatically             */
                seq_t sum1 = 0, sum2 = 0, sum3 = 0;

                /* Σ_{i=1}^{k-1} T(n+i, k-i)  (same diagonal, larger n)   */
                for (idx_t i = 1; i < k_curr; ++i) {
                    seq_t v = dp[(n_curr + i) * stride + (k_curr - i)];
                    sum1 += v;
                }
                /* Σ_{i=1}^{min(n,k)-1} T(n-i, k-i) (lower diagonals) */
                idx_t lim = (n_curr < k_curr) ? n_curr : k_curr;
                if (lim > 1) {                 /* add only if min(n,k)>1      */
                    for (idx_t i = 1; i <= lim - 1; ++i) {
                        seq_t v = dp[(n_curr - i) * stride + (k_curr - i)];
                        sum2 += v;
                    }
                }
                /* Σ_{i=1}^{n-1} T(n-i, k)  (lower diagonal)          */
                for (idx_t i = 1; i < n_curr; ++i) {
                    seq_t v = dp[(n_curr - i) * stride + k_curr];
                    sum3 += v;
                }
                dp[n_curr * stride + k_curr] = sum1 + sum2 + sum3;
            }
        }
        cur_max_sum = s;
    }

    return dp[n_coord * stride + k_coord];
}
