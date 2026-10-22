/*
 * A361860  Number of integer partitions of n whose median part is the
 *          smallest.
 * https://oeis.org/A361860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1}^{⌊n/2⌋} ( Σ_{m≥1, 2k·m ≤ n} p_{k-1}(n-2k·m) +
 *            Σ_{m≥1, (2k+1)·m ≤ n} p_k(n-(2k+1)·m) ) where p_k(s) is the
 *            number of partitions of s into at most k parts (equivalently,
 *            the number of partitions of s with largest part ≤ k). The
 *            function below computes a(n) by dynamic programming
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; never called for 0   */

    idx_t N = n;
    /* dp[t] will hold p_k(t) after the k‑th iteration (parts ≤ k)   */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return 0;                  /* memory failure – unlikely          */

    dp[0] = 1;                          /* p_0(0) = 1, p_0(t>0) = 0          */
    for (idx_t i = 1; i <= N; ++i) dp[i] = 0;

    seq_t ans = 1;                      /* single‑part partition  a = n         */

    for (idx_t k = 1; k <= N / 2; ++k)
    {
        /* Even length partitions – use p_{k-1} (dp before update)    */
        for (idx_t m = 1;; ++m)
        {
            idx_t offset = m * (2 * k);          /* 2k·m   */
            if (offset > N) break;
            ans += dp[N - offset];
        }

        /* Update dp to include part size k (now parts ≤ k)          */
        for (idx_t t = k; t <= N; ++t)
            dp[t] += dp[t - k];

        /* Odd length partitions – use p_k  (dp after update)        */
        for (idx_t m = 1;; ++m)
        {
            idx_t offset = m * (2 * k + 1);      /* (2k+1)·m */
            if (offset > N) break;
            ans += dp[N - offset];
        }
    }

    free(dp);
    return ans;
}
