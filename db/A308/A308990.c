/*
 * A308990  Sum of the smallest parts in the partitions of n into 8 parts.
 * https://oeis.org/A308990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p=1..floor(n/8)} p * P_7(n-8p) where P_7(m) is the
 *            number of partitions of m into parts of size 1..7 (equivalently,
 *            partitions of m into at most 7 parts)
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n < 8 there are no 8‑part partitions. */
    if (n < 8)
        return (seq_t)0;

    /* We need partitions of all m = n-8p with 1 ≤ p ≤ n/8.
       The largest such m is n-8 (when p=1). */
    idx_t max_m = n - 8;

    /* Allocate DP array for P_7[0..max_m] */
    seq_t *dp = (seq_t *)malloc((max_m + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* Out‑of‑memory: safe‑return */

    /* Initialize DP: P_7[0] = 1 (empty partition), others 0 */
    for (idx_t i = 0; i <= max_m; ++i)
        dp[i] = 0;
    dp[0] = (seq_t)1;

    /* Unbounded knapsack DP to compute partitions into parts 1..7. */
    for (idx_t part = 1; part <= 7; ++part) {
        for (idx_t i = part; i <= max_m; ++i) {
            dp[i] += dp[i - part];
        }
    }

    /* Accumulate the required sum of smallest parts. */
    seq_t sum = 0;
    for (idx_t p = 1; p <= n / 8; ++p) {
        idx_t m = n - 8 * p;
        sum += (seq_t)p * dp[m];
    }

    free(dp);
    return sum;
}
