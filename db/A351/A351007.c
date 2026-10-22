/*
 * A351007  Number of even-length integer partitions of n into parts that are
 *          alternately unequal and equal.
 * https://oeis.org/A351007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even‑length partitions of n into parts whose
 *            multiplicities start with 1, are followed by any number of 2's,
 *            and end with another 1. The algorithm computes the count by
 *            iterating over the number of distinct parts r, building the
 *            coin‑change DP for the corresponding odd weights, a...
 * Range:     u8 n <= 44, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Value for n = 0 is defined as 1 by OEIS. */
    if (n == 0)
        return (seq_t)1;

    seq_t answer = (seq_t)0;

    /* r is the number of distinct parts. Minimum r is 2. */
    for (idx_t r = 2; ; ++r) {
        /* Minimum possible sum with this r is r^2 - 1.
           If it already exceeds n, break. */
        if (r * r > n + 1)
            break;

        idx_t r_minus1 = r - 1;

        /* Dynamic programming array for the coin‑change problem.
           dp[t] = number of ways to obtain sum t using the allowed
           odd weights for this r.  Size is n+1. */
        seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!dp) return (seq_t)0; /* allocation failed */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* Weights w_k = 2*(r-k)+1 for k = 2..r.  These are odd numbers
           decreasing from 2*r-3 down to 1.  Unbounded coin change. */
        for (idx_t k = 2; k <= r; ++k) {
            idx_t weight = 2 * (r - k) + 1;
            for (idx_t t = weight; t <= n; ++t)
                dp[t] += dp[t - weight];
        }

        /* Sum over all possible first parts x = a1 >= 1.
           For each x compute T = n - 2(r-1)x - (r-1)^2.
           If T >= 0, add dp[T] to the answer. */
        for (idx_t x = 1; ; ++x) {
            long long T =
                (long long)n - (long long)2 * (long long)r_minus1 * (long long)x
                - (long long)r_minus1 * (long long)r_minus1;
            if (T < 0)
                break;
            answer += dp[(idx_t)T];
        }

        free(dp);
    }

    return answer;
}
