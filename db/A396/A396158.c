/*
 * A396158  Triangle; T(n,m) is the number of possible outcomes having a run
 *          of m or more sixes when a die is rolled n times.
 * https://oeis.org/A396158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row‑major flatten of the triangle T(n,m) where T(n,m)
 *            counts sequences of length n with a run of m or more sixes.
 *            Computed by dynamic programming
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <math.h>
#include <stdlib.h>

static seq_t compute_T(idx_t n, idx_t m)
{
    /* Exact values for the trivial cases */
    if (m == 0) {
        seq_t pow6 = 1;
        for (idx_t i = 0; i < n; ++i) pow6 *= 6;
        return pow6;
    }
    if (m > n) return 0;          /* impossible run length */
    if (m == n) return 1;         /* all sixes only */

    /* Maximum possible run length for the problems we must handle
       (n <= about 10000 gives n <= 141, so 200 is plenty). */
    enum { MAX_M = 200 };
    if (m > MAX_M) {
        /* Fallback dynamic allocation for the unlikely event of a larger m */
        seq_t *dp = (seq_t*)malloc((size_t)m * sizeof(seq_t));
        if (!dp) return 0;          /* allocation failed */
        for (idx_t i = 0; i < m; ++i) dp[i] = 0;
        dp[0] = 1;
        seq_t sumdp = 1;
        for (idx_t i = 1; i <= n; ++i) {
            seq_t old_last = dp[m-1];
            for (idx_t k = m-1; k > 0; --k) dp[k] = dp[k-1];
            dp[0] = sumdp * 5;
            sumdp = sumdp * 6 - old_last;
        }
        seq_t f = sumdp;
        seq_t pow6 = 1;
        for (idx_t i = 0; i < n; ++i) pow6 *= 6;
        seq_t result = pow6 - f;
        free(dp);
        return result;
    }

    /* Fixed array implementation (safe for all allowed indices). */
    seq_t dp[MAX_M];
    for (idx_t i = 0; i < m; ++i) dp[i] = 0;
    dp[0] = 1;
    seq_t sumdp = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t old_last = dp[m-1];
        for (idx_t k = m-1; k > 0; --k) dp[k] = dp[k-1];
        dp[0] = sumdp * 5;
        sumdp = sumdp * 6 - old_last;
    }
    seq_t f = sumdp;
    seq_t pow6 = 1;
    for (idx_t i = 0; i < n; ++i) pow6 *= 6;
    return pow6 - f;
}

seq_t a(idx_t n)
{
    /* Find row r and column c with 0 ≤ c ≤ r such that
       n = r*(r+1)/2 + c. */
    idx_t r;
    {
        double tmp = sqrt((double)8 * n + 1);
        r = (idx_t)((tmp - 1.0) / 2.0);
        while ((r + 1) * (r + 2) / 2 <= n) ++r;
        while (r * (r + 1) / 2 > n) --r;
    }
    idx_t start = r * (r + 1) / 2;
    idx_t m = n - start;            /* column (run length) */
    return compute_T(r, m);
}
